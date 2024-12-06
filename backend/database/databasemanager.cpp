#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QString>

QString hashPassword(const QString& password) {
    QString salt = "fixed_salt_value"; // 使用固定盐值
    QString passwordWithSalt = password + salt;
    QByteArray hash;
    for (int i = 0; i < 10000; ++i) {
        hash = QCryptographicHash::hash(
            passwordWithSalt.toUtf8(),
            QCryptographicHash::Sha256
            );
        passwordWithSalt = hash.toHex();
    }
    return salt + "$" + QString::fromUtf8(hash.toHex());
}

DatabaseManager::DatabaseManager() {
    QFile configFile(":/config/database_config.json");
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = configFile.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject config = jsonDoc.object();

            // 检查配置项是否存在
            if (config.contains("hostname") && config.contains("port") &&
                config.contains("database") && config.contains("username") &&
                config.contains("password")) {
                m_db = QSqlDatabase::addDatabase("QODBC");
                m_db.setHostName(config["hostname"].toString());
                m_db.setPort(config["port"].toInt());
                m_db.setDatabaseName(config["database"].toString());
                m_db.setUserName(config["username"].toString());
                m_db.setPassword(config["password"].toString());
            } else {
                qDebug() << "Error: Incomplete database configuration";
            }
        } else {
            qDebug() << "Error: Invalid JSON format";
        }

        configFile.close();
    } else {
        qDebug() << "Error: Could not open config file" << configFile.errorString();
    }
}

bool DatabaseManager::connectToDatabase() {
    // 尝试打开数据库连接
    if (!m_db.open()) {
        qDebug() << "Error: Connection failed" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Database connected successfully!";
    return true;
}

void DatabaseManager::createTable() {
    QSqlQuery query;

    // 用户
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INT AUTO_INCREMENT PRIMARY KEY, "
                    "username VARCHAR(50) NOT NULL, "
                    "telephone VARCHAR(15) NOT NULL, "
                    "password VARCHAR(100) NOT NULL)")){
        qDebug() << "create users error: " << query.lastError().text();
    }

    // 航班信息
    if(!query.exec("CREATE TABLE IF NOT EXISTS flight_info ("
                    "flight_id INTEGER PRIMARY KEY AUTO_INCREMENT, "   // 航班ID，自增主键
                    "flight_number VARCHAR(10) NOT NULL, "             // 航班号
                    "departure_city VARCHAR(20) NOT NULL, "            // 起点城市
                    "arrival_city VARCHAR(20) NOT NULL, "              // 终点城市
                    "price DECIMAL(10,2) NOT NULL, "                   // 票价（最多10位数字，两位小数）
                    "departure_airport VARCHAR(20) NOT NULL, "         // 起点机场
                    "arrival_airport VARCHAR(20) NOT NULL, "           // 终点机场
                    "airline_company VARCHAR(20) NOT NULL, "           // 航空公司
                    "checkin_start_time DATETIME NOT NULL, "           // 检票开始时间
                    "checkin_end_time DATETIME NOT NULL, "             // 检票结束时间
                    "status VARCHAR(10) NOT NULL, "                    // 航班状态
                    "UNIQUE(flight_number) )")) {                      // 确保航班号唯一
        qDebug() << "create flight_info error: " << query.lastError().text();
    }

    // 所有订单
    if(!query.exec("CREATE TABLE IF NOT EXISTS all_order ("
                    "id INT AUTO_INCREMENT PRIMARY KEY, "
                    "user_id INT NOT NULL, "
                    "flight_id INT NOT NULL, "
                    "order_time DATETIME DEFAULT CURRENT_TIMESTAMP, "
                    "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE, "
                    "FOREIGN KEY (flight_id) REFERENCES flight_info(flight_id) ON DELETE CASCADE)")){
        qDebug() << "create all_order error: " << query.lastError().text();
    }
}

bool DatabaseManager::insertUser(const QString& username, const QString& telephone, const QString& password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, telephone, password) VALUES (:username, :telephone, :password)");
    QString hashedPassword = hashPassword(password);
    qDebug() << "wwwww" << password << " " << hashedPassword;
    query.bindValue(":username", username);
    query.bindValue(":telephone", telephone);
    query.bindValue(":password",hashedPassword);

    if(!query.exec()) {
        qDebug() << "Insert error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::insertFlight(const QString& flightNumber, const QString& departureCity, const QString& arrivalCity,
                                   double price, const QString& departureAirport, const QString& arrivalAirport,
                                   const QString& airlineCompany, const QDateTime& checkinStartTime,
                                   const QDateTime& checkinEndTime, const QString& status) {
    QSqlQuery query;
    query.prepare("INSERT INTO flight_info (flight_number, departure_city, arrival_city, price, "
                  "departure_airport, arrival_airport, airline_company, checkin_start_time, "
                  "checkin_end_time, status) "
                  "VALUES (:flight_number, :departure_city, :arrival_city, :price, "
                  ":departure_airport, :arrival_airport, :airline_company, :checkin_start_time, "
                  ":checkin_end_time, :status)");
    query.bindValue(":flight_number", flightNumber);
    query.bindValue(":departure_city", departureCity);
    query.bindValue(":arrival_city", arrivalCity);
    query.bindValue(":price", price);
    query.bindValue(":departure_airport", departureAirport);
    query.bindValue(":arrival_airport", arrivalAirport);
    query.bindValue(":airline_company", airlineCompany);
    query.bindValue(":checkin_start_time", checkinStartTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":checkin_end_time", checkinEndTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":status", status);

    if(!query.exec()) {
        qDebug() << "Insert flight error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::queryUsers(const QString& telephone){
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE telephone = :phone");
    query.bindValue(":phone", telephone);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool DatabaseManager::queryUsers(const QString& telephone, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE telephone = :phone AND password = :pwd");
    query.bindValue(":phone", telephone);
    QString hashedPassword = hashPassword(password);
    qDebug() << "login" << password << " " << hashedPassword;
    query.bindValue(":pwd", hashedPassword);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool DatabaseManager::isFlightInfoEmpty() const {
    QSqlQuery query("SELECT COUNT(*) FROM flight_info");
    if (query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "flight_info 表中的记录数：" << count;
        return count == 0;
    }
    qDebug() << "查询 flight_info 表失败：" << query.lastError().text();
    return true; // 如果查询失败，默认认为为空，避免插入失败
}


/*** 测试函数 ***/
void DatabaseManager::populateSampleFlights() {
    // 检查 flight_info 表是否为空，避免重复插入
    if (!isFlightInfoEmpty()) {
        qDebug() << "flight_info 表已包含数据，跳过示例数据插入。";
        return;
    }

    qDebug() << "flight_info 表为空，开始插入示例航班数据。";

    // 定义30条示例航班数据
    QList<QVariantList> flights = {
        {"CA123", "北京", "上海", 1800.00, "北京首都国际机场", "上海浦东国际机场", "中国国航", QDateTime::fromString("2024-05-01 07:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-01 09:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU456", "北京", "广州", 2000.00, "北京首都国际机场", "广州白云国际机场", "东方航空", QDateTime::fromString("2024-05-02 08:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-02 10:30:00", "yyyy-MM-dd HH:mm:ss"), "Delayed"},
        {"HU789", "上海", "深圳", 1500.00, "上海浦东国际机场", "深圳宝安国际机场", "海南航空", QDateTime::fromString("2024-05-03 09:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-03 11:30:00", "yyyy-MM-dd HH:mm:ss"), "Cancelled"},
        {"CZ101", "成都", "杭州", 1700.00, "成都双流国际机场", "杭州萧山国际机场", "中国南方航空", QDateTime::fromString("2024-05-04 06:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-04 08:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"ZH202", "武汉", "西安", 1600.00, "武汉天河国际机场", "西安咸阳国际机场", "深圳航空", QDateTime::fromString("2024-05-05 10:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-05 12:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"JD303", "重庆", "昆明", 1900.00, "重庆江北国际机场", "昆明长水国际机场", "吉祥航空", QDateTime::fromString("2024-05-06 14:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-06 16:30:00", "yyyy-MM-dd HH:mm:ss"), "Delayed"},
        {"FM404", "深圳", "北京", 2100.00, "深圳宝安国际机场", "北京首都国际机场", "春秋航空", QDateTime::fromString("2024-05-07 15:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-07 17:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SA505", "南京", "厦门", 1400.00, "南京禄口国际机场", "厦门高崎国际机场", "四川航空", QDateTime::fromString("2024-05-08 12:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-08 14:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SN606", "杭州", "广州", 1750.00, "杭州萧山国际机场", "广州白云国际机场", "山东航空", QDateTime::fromString("2024-05-09 13:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-09 15:30:00", "yyyy-MM-dd HH:mm:ss"), "Cancelled"},
        {"ZH707", "上海", "北京", 1650.00, "上海浦东国际机场", "北京首都国际机场", "深圳航空", QDateTime::fromString("2024-05-10 09:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-10 11:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU808", "北京", "成都", 1850.00, "北京首都国际机场", "成都双流国际机场", "东方航空", QDateTime::fromString("2024-05-11 07:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-11 10:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"CA909", "广州", "上海", 1950.00, "广州白云国际机场", "上海浦东国际机场", "中国国航", QDateTime::fromString("2024-05-12 11:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-12 13:30:00", "yyyy-MM-dd HH:mm:ss"), "Delayed"},
        {"HU010", "昆明", "北京", 2200.00, "昆明长水国际机场", "北京首都国际机场", "海南航空", QDateTime::fromString("2024-05-13 16:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-13 18:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"FM111", "西安", "重庆", 1550.00, "西安咸阳国际机场", "重庆江北国际机场", "春秋航空", QDateTime::fromString("2024-05-14 10:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-14 13:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SA212", "厦门", "杭州", 1450.00, "厦门高崎国际机场", "杭州萧山国际机场", "四川航空", QDateTime::fromString("2024-05-15 08:45:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-15 11:15:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SN313", "广州", "南京", 1600.00, "广州白云国际机场", "南京禄口国际机场", "山东航空", QDateTime::fromString("2024-05-16 14:15:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-16 16:45:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"ZH414", "北京", "杭州", 1750.00, "北京首都国际机场", "杭州萧山国际机场", "深圳航空", QDateTime::fromString("2024-05-17 07:15:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-17 09:45:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU515", "成都", "广州", 2050.00, "成都双流国际机场", "广州白云国际机场", "东方航空", QDateTime::fromString("2024-05-18 12:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-18 15:00:00", "yyyy-MM-dd HH:mm:ss"), "Delayed"},
        {"CA616", "上海", "北京", 1850.00, "上海浦东国际机场", "北京首都国际机场", "中国国航", QDateTime::fromString("2024-05-19 09:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-19 12:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"HU717", "深圳", "成都", 2100.00, "深圳宝安国际机场", "成都双流国际机场", "海南航空", QDateTime::fromString("2024-05-20 13:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-20 15:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"FM818", "杭州", "西安", 1650.00, "杭州萧山国际机场", "西安咸阳国际机场", "春秋航空", QDateTime::fromString("2024-05-21 10:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-21 12:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SA919", "南京", "北京", 1500.00, "南京禄口国际机场", "北京首都国际机场", "四川航空", QDateTime::fromString("2024-05-22 11:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-22 14:00:00", "yyyy-MM-dd HH:mm:ss"), "Cancelled"},
        {"SN020", "北京", "重庆", 1750.00, "北京首都国际机场", "重庆江北国际机场", "山东航空", QDateTime::fromString("2024-05-23 06:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-23 08:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"ZH121", "广州", "昆明", 1900.00, "广州白云国际机场", "昆明长水国际机场", "深圳航空", QDateTime::fromString("2024-05-24 15:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-24 18:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU222", "成都", "上海", 1950.00, "成都双流国际机场", "上海浦东国际机场", "东方航空", QDateTime::fromString("2024-05-25 07:45:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-25 10:15:00", "yyyy-MM-dd HH:mm:ss"), "Delayed"},
        {"CA323", "西安", "深圳", 1600.00, "西安咸阳国际机场", "深圳宝安国际机场", "中国国航", QDateTime::fromString("2024-05-26 08:15:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-26 10:45:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"HU424", "昆明", "杭州", 1850.00, "昆明长水国际机场", "杭州萧山国际机场", "海南航空", QDateTime::fromString("2024-05-27 14:45:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-27 17:15:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"FM525", "深圳", "南京", 1700.00, "深圳宝安国际机场", "南京禄口国际机场", "春秋航空", QDateTime::fromString("2024-05-28 16:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-28 18:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SA626", "上海", "重庆", 2000.00, "上海浦东国际机场", "重庆江北国际机场", "四川航空", QDateTime::fromString("2024-05-29 09:15:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-29 11:45:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SN727", "杭州", "广州", 1550.00, "杭州萧山国际机场", "广州白云国际机场", "山东航空", QDateTime::fromString("2024-05-30 12:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-30 15:00:00", "yyyy-MM-dd HH:mm:ss"), "Cancelled"},
        {"ZH828", "北京", "西安", 1800.00, "北京首都国际机场", "西安咸阳国际机场", "深圳航空", QDateTime::fromString("2024-05-31 07:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-05-31 09:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU929", "重庆", "北京", 2050.00, "重庆江北国际机场", "北京首都国际机场", "东方航空", QDateTime::fromString("2024-06-01 13:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-01 15:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"CA030", "广州", "成都", 1900.00, "广州白云国际机场", "成都双流国际机场", "中国国航", QDateTime::fromString("2024-06-02 10:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-02 13:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"HU131", "西安", "杭州", 1600.00, "西安咸阳国际机场", "杭州萧山国际机场", "海南航空", QDateTime::fromString("2024-06-03 11:45:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-03 14:15:00", "yyyy-MM-dd HH:mm:ss"), "Delayed"},
        {"FM232", "南京", "成都", 1750.00, "南京禄口国际机场", "成都双流国际机场", "春秋航空", QDateTime::fromString("2024-06-04 08:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-04 11:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SA333", "昆明", "深圳", 2000.00, "昆明长水国际机场", "深圳宝安国际机场", "四川航空", QDateTime::fromString("2024-06-05 14:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-05 16:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SN434", "上海", "杭州", 1500.00, "上海浦东国际机场", "杭州萧山国际机场", "山东航空", QDateTime::fromString("2024-06-06 09:45:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-06 12:15:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"ZH535", "北京", "广州", 1850.00, "北京首都国际机场", "广州白云国际机场", "深圳航空", QDateTime::fromString("2024-06-07 07:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-07 10:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU636", "成都", "杭州", 1950.00, "成都双流国际机场", "杭州萧山国际机场", "东方航空", QDateTime::fromString("2024-06-08 12:15:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-08 14:45:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"CA737", "西安", "广州", 1700.00, "西安咸阳国际机场", "广州白云国际机场", "中国国航", QDateTime::fromString("2024-06-09 10:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-09 12:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"HU838", "深圳", "上海", 2100.00, "深圳宝安国际机场", "上海浦东国际机场", "海南航空", QDateTime::fromString("2024-06-10 15:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-10 18:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"FM939", "杭州", "重庆", 1600.00, "杭州萧山国际机场", "重庆江北国际机场", "春秋航空", QDateTime::fromString("2024-06-11 11:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-11 13:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SA040", "南京", "北京", 1550.00, "南京禄口国际机场", "北京首都国际机场", "四川航空", QDateTime::fromString("2024-06-12 08:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-12 10:30:00", "yyyy-MM-dd HH:mm:ss"), "Cancelled"},
        {"SN141", "北京", "杭州", 1750.00, "北京首都国际机场", "杭州萧山国际机场", "山东航空", QDateTime::fromString("2024-06-13 07:15:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-13 09:45:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"ZH242", "广州", "成都", 1800.00, "广州白云国际机场", "成都双流国际机场", "深圳航空", QDateTime::fromString("2024-06-14 14:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-14 17:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU343", "西安", "深圳", 1650.00, "西安咸阳国际机场", "深圳宝安国际机场", "东方航空", QDateTime::fromString("2024-06-15 09:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-15 11:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"CA444", "上海", "北京", 1900.00, "上海浦东国际机场", "北京首都国际机场", "中国国航", QDateTime::fromString("2024-06-16 10:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-16 13:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"HU545", "成都", "杭州", 1750.00, "成都双流国际机场", "杭州萧山国际机场", "海南航空", QDateTime::fromString("2024-06-17 12:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-17 14:30:00", "yyyy-MM-dd HH:mm:ss"), "Delayed"},
        {"FM646", "南京", "广州", 1600.00, "南京禄口国际机场", "广州白云国际机场", "春秋航空", QDateTime::fromString("2024-06-18 11:15:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-18 13:45:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SA747", "昆明", "北京", 2050.00, "昆明长水国际机场", "北京首都国际机场", "四川航空", QDateTime::fromString("2024-06-19 16:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-19 18:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"SN848", "北京", "成都", 1850.00, "北京首都国际机场", "成都双流国际机场", "山东航空", QDateTime::fromString("2024-06-20 07:45:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-20 10:15:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"ZH949", "广州", "西安", 2000.00, "广州白云国际机场", "西安咸阳国际机场", "深圳航空", QDateTime::fromString("2024-06-21 13:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-21 16:00:00", "yyyy-MM-dd HH:mm:ss"), "On Time"},
        {"MU050", "杭州", "深圳", 1700.00, "杭州萧山国际机场", "深圳宝安国际机场", "东方航空", QDateTime::fromString("2024-06-22 10:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-06-22 12:30:00", "yyyy-MM-dd HH:mm:ss"), "On Time"}
    };

    // 遍历并插入每条航班数据
    foreach (const QVariantList &flight, flights) {
        bool success = insertFlight(
            flight[0].toString(),  // flight_number
            flight[1].toString(),  // departure_city
            flight[2].toString(),  // arrival_city
            flight[3].toDouble(),  // price
            flight[4].toString(),  // departure_airport
            flight[5].toString(),  // arrival_airport
            flight[6].toString(),  // airline_company
            flight[7].toDateTime(), // checkin_start_time
            flight[8].toDateTime(), // checkin_end_time
            flight[9].toString()   // status
            );

        if (!success) {
            qDebug() << "Failed to insert flight:" << flight[0].toString();
        }
    }

    qDebug() << "Sample flights population completed.";
}


DatabaseManager::~DatabaseManager() {
    // 关闭数据库连接
    if(m_db.isOpen()) {
        m_db.close();
    }
}
