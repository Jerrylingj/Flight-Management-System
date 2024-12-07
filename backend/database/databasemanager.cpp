#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QString>
#include <stdexcept>

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
                    "password VARCHAR(100) NOT NULL, "
                    "avartar_url VARCHAR(255), "
                    "balance DECIMAL(10, 2) DEFAULT 0.00, "
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)")){
        qDebug() << "create users error: " << query.lastError().text();
    }

    // 航班信息
    if(!query.exec("CREATE TABLE IF NOT EXISTS flight_info ("
                    "flight_id INTEGER PRIMARY KEY AUTO_INCREMENT, "   // 航班ID，自增主键
                    "flight_number VARCHAR(10) NOT NULL, "             // 航班号
                    "departure_city VARCHAR(20) NOT NULL, "            // 起点城市
                    "arrival_city VARCHAR(20) NOT NULL, "              // 终点城市
                    "departure_time DATETIME NOT NULL, "               // 出发时间
                    "arrival_time DATETIME NOT NULL, "                 // 到达时间
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
    query.bindValue(":username", username);
    query.bindValue(":telephone", telephone);
    query.bindValue(":password",hashedPassword);

    if(!query.exec()) {
        qDebug() << "Insert error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::insertFlight(const QString& flightNumber, const QString& departureCity, const QString& arrivalCity,const QDateTime& departureTime, const QDateTime& arrivalTime,
                                   double price, const QString& departureAirport, const QString& arrivalAirport,
                                   const QString& airlineCompany, const QDateTime& checkinStartTime,
                                   const QDateTime& checkinEndTime, const QString& status) {
    QSqlQuery query;
    query.prepare("INSERT INTO flight_info (flight_number, departure_city, arrival_city, departure_time, arrival_time, price, departure_airport, arrival_airport, checkin_start_time, checkin_end_time, airline_company, status) "
                  "VALUES (:flight_number, :departure_city, :arrival_city, :departure_time, :arrival_time, :price, :departure_airport, :arrival_airport, :checkin_start_time, :checkin_end_time, :airline_company, :status)");
    query.bindValue(":flight_number", flightNumber);
    query.bindValue(":departure_city", departureCity);
    query.bindValue(":arrival_city", arrivalCity);
    query.bindValue(":departure_time", departureTime);
    query.bindValue(":arrival_time", arrivalTime);
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

int DatabaseManager::queryUsers(const QString& telephone, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE telephone = :phone AND password = :pwd");
    query.bindValue(":phone", telephone);
    QString hashedPassword = hashPassword(password);
    qDebug() << "login" << password << " " << hashedPassword;
    query.bindValue(":pwd", hashedPassword);

    if (query.exec() && query.next()) {
        int id = query.value("id").toInt();
        return id;
    }

    return -1;
}

void DatabaseManager::queryFlight(int flightId,FlightInfo& flight){
    QString sql = "SELECT * FROM flight_info WHERE flight_id = :flight_id";
    qDebug() << "Preparing SQL:" << sql << "with flight_id =" << flightId;

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":flight_id", flightId);

    if (!query.exec()) {
        qDebug() << "查询失败:" << query.lastError().text();
        throw std::runtime_error("Query execution failed: " + query.lastError().text().toStdString());
    }

    if (query.next()) {
        flight.flightId = query.value("flight_id").toInt();
        flight.flightNumber = query.value("flight_number").toString();
        flight.departureCity = query.value("departure_city").toString();
        flight.arrivalCity = query.value("arrival_city").toString();
        flight.departureAirport = query.value("departure_airport").toString();
        flight.arrivalAirport = query.value("arrival_airport").toString();
        flight.checkinStartTime = query.value("checkin_start_time").toDateTime();
        flight.checkinEndTime = query.value("checkin_end_time").toDateTime();
        flight.price = query.value("price").toDouble();
        flight.airlineCompany = query.value("airline_company").toString();
        flight.status = query.value("status").toString();

        qDebug() << "Flight found:" << flight.flightNumber;
    } else {
        qDebug() << "No flight found with flight_id =" << flightId;
        throw std::invalid_argument("无效的id");
    }
}

void DatabaseManager::queryFlight(QJsonArray& flights){
    QString sql = "SELECT * FROM flight_info";
    qDebug() << "Executing SQL:" << sql;

    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "查询失败:" << query.lastError().text();
        throw std::runtime_error("查询失败");
    } else {
        while (query.next()) {
            FlightInfo flight;
            flight.flightId = query.value("flight_id").toInt();
            flight.flightNumber = query.value("flight_number").toString();
            flight.departureCity = query.value("departure_city").toString();
            flight.arrivalCity = query.value("arrival_city").toString();
            flight.departureAirport = query.value("departure_airport").toString();
            flight.arrivalAirport = query.value("arrival_airport").toString();
            flight.checkinStartTime = query.value("checkin_start_time").toDateTime();
            flight.checkinEndTime = query.value("checkin_end_time").toDateTime();
            flight.price = query.value("price").toDouble();
            flight.airlineCompany = query.value("airline_company").toString();
            flight.status = query.value("status").toString();

            flights.append(flight.toJson());
        }
    }

    qDebug() << "Retrieved" << flights.size() << "flights.";
    if(flights.isEmpty()){
        throw std::runtime_error("没有数据");
    }
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


void DatabaseManager::populateSampleFlights() {
    // 清空表格
    QSqlQuery queryClear;
    if (!queryClear.exec("DELETE FROM flight_info")) {
        qDebug() << "Failed to clear flight_info table:" << queryClear.lastError();
        return;
    }
    qDebug() << "flight_info 表已清空。";

    // 开始事务
    if (!QSqlDatabase::database().transaction()) {
        qDebug() << "Failed to start transaction";
        return;
    }

    QList<QVariantList> flights = {
        {"MU100", "北京", "上海", QDateTime::fromString("2024-12-08 07:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 09:00:00", "yyyy-MM-dd HH:mm:ss"), 1500.00, "北京首都国际机场", "上海虹桥国际机场", QDateTime::fromString("2024-12-08 06:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 07:00:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "On Time"},
        {"MU200", "北京", "广州", QDateTime::fromString("2024-12-08 10:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 12:00:00", "yyyy-MM-dd HH:mm:ss"), 1600.00, "北京首都国际机场", "广州白云国际机场", QDateTime::fromString("2024-12-08 08:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 09:30:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "On Time"},
        {"CA300", "上海", "成都", QDateTime::fromString("2024-12-08 08:45:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 11:30:00", "yyyy-MM-dd HH:mm:ss"), 1800.00, "上海浦东国际机场", "成都双流国际机场", QDateTime::fromString("2024-12-08 07:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 08:00:00", "yyyy-MM-dd HH:mm:ss"), "中国国际航空", "Delayed"},
        {"MU400", "南京", "青岛", QDateTime::fromString("2024-12-08 09:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 11:30:00", "yyyy-MM-dd HH:mm:ss"), 1700.00, "南京禄口国际机场", "青岛流亭国际机场", QDateTime::fromString("2024-12-08 07:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 08:30:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "On Time"},
        {"ZH500", "深圳", "杭州", QDateTime::fromString("2024-12-08 10:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 12:00:00", "yyyy-MM-dd HH:mm:ss"), 1500.00, "深圳宝安国际机场", "杭州萧山国际机场", QDateTime::fromString("2024-12-08 09:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 10:00:00", "yyyy-MM-dd HH:mm:ss"), "春秋航空", "On Time"},
        {"CA600", "上海", "重庆", QDateTime::fromString("2024-12-08 12:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 14:00:00", "yyyy-MM-dd HH:mm:ss"), 1900.00, "上海浦东国际机场", "重庆江北国际机场", QDateTime::fromString("2024-12-08 10:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 11:30:00", "yyyy-MM-dd HH:mm:ss"), "中国国际航空", "On Time"},
        {"MU700", "北京", "深圳", QDateTime::fromString("2024-12-08 14:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 16:00:00", "yyyy-MM-dd HH:mm:ss"), 2000.00, "北京首都国际机场", "深圳宝安国际机场", QDateTime::fromString("2024-12-08 12:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 13:30:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "Delayed"},
        {"ZH800", "广州", "上海", QDateTime::fromString("2024-12-08 15:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 17:00:00", "yyyy-MM-dd HH:mm:ss"), 1800.00, "广州白云国际机场", "上海虹桥国际机场", QDateTime::fromString("2024-12-08 13:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 14:30:00", "yyyy-MM-dd HH:mm:ss"), "春秋航空", "On Time"},
        {"CA900", "成都", "西安", QDateTime::fromString("2024-12-08 16:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 17:30:00", "yyyy-MM-dd HH:mm:ss"), 1700.00, "成都双流国际机场", "西安咸阳国际机场", QDateTime::fromString("2024-12-08 14:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 15:30:00", "yyyy-MM-dd HH:mm:ss"), "中国国际航空", "On Time"},
        {"MU1000", "广州", "杭州", QDateTime::fromString("2024-12-08 17:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 19:00:00", "yyyy-MM-dd HH:mm:ss"), 1600.00, "广州白云国际机场", "杭州萧山国际机场", QDateTime::fromString("2024-12-08 15:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 16:30:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "On Time"},
        {"ZH1100", "上海", "南京", QDateTime::fromString("2024-12-08 18:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 19:00:00", "yyyy-MM-dd HH:mm:ss"), 1500.00, "上海虹桥国际机场", "南京禄口国际机场", QDateTime::fromString("2024-12-08 16:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 17:30:00", "yyyy-MM-dd HH:mm:ss"), "春秋航空", "Delayed"},
        {"MU1200", "北京", "杭州", QDateTime::fromString("2024-12-08 19:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 21:00:00", "yyyy-MM-dd HH:mm:ss"), 1700.00, "北京首都国际机场", "杭州萧山国际机场", QDateTime::fromString("2024-12-08 17:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 18:30:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "On Time"},
        {"ZH1300", "上海", "深圳", QDateTime::fromString("2024-12-08 20:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 22:00:00", "yyyy-MM-dd HH:mm:ss"), 1900.00, "上海浦东国际机场", "深圳宝安国际机场", QDateTime::fromString("2024-12-08 18:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 19:30:00", "yyyy-MM-dd HH:mm:ss"), "中国国际航空", "On Time"},
        {"MU1400", "北京", "武汉", QDateTime::fromString("2024-12-08 21:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 23:00:00", "yyyy-MM-dd HH:mm:ss"), 1600.00, "北京首都国际机场", "武汉天河国际机场", QDateTime::fromString("2024-12-08 19:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 20:00:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "On Time"},
        {"CA1500", "上海", "重庆", QDateTime::fromString("2024-12-08 22:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 23:30:00", "yyyy-MM-dd HH:mm:ss"), 1800.00, "上海浦东国际机场", "重庆江北国际机场", QDateTime::fromString("2024-12-08 20:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-08 21:30:00", "yyyy-MM-dd HH:mm:ss"), "中国国际航空", "Delayed"},
        {"MU1600", "广州", "上海", QDateTime::fromString("2024-12-09 06:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 08:00:00", "yyyy-MM-dd HH:mm:ss"), 1700.00, "广州白云国际机场", "上海虹桥国际机场", QDateTime::fromString("2024-12-09 04:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 05:30:00", "yyyy-MM-dd HH:mm:ss"), "春秋航空", "On Time"},
        {"ZH1700", "成都", "南京", QDateTime::fromString("2024-12-09 07:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 08:30:00", "yyyy-MM-dd HH:mm:ss"), 1500.00, "成都双流国际机场", "南京禄口国际机场", QDateTime::fromString("2024-12-09 05:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 06:30:00", "yyyy-MM-dd HH:mm:ss"), "春秋航空", "On Time"},
        {"MU1800", "武汉", "青岛", QDateTime::fromString("2024-12-09 08:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 10:30:00", "yyyy-MM-dd HH:mm:ss"), 1600.00, "武汉天河国际机场", "青岛流亭国际机场", QDateTime::fromString("2024-12-09 06:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 07:30:00", "yyyy-MM-dd HH:mm:ss"), "东方航空", "Delayed"},
        {"ZH1900", "上海", "西安", QDateTime::fromString("2024-12-09 09:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 11:00:00", "yyyy-MM-dd HH:mm:ss"), 1700.00, "上海浦东国际机场", "西安咸阳国际机场", QDateTime::fromString("2024-12-09 07:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 08:30:00", "yyyy-MM-dd HH:mm:ss"), "春秋航空", "On Time"},
        {"CA2000", "北京", "深圳", QDateTime::fromString("2024-12-09 10:00:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 12:00:00", "yyyy-MM-dd HH:mm:ss"), 1800.00, "北京首都国际机场", "深圳宝安国际机场", QDateTime::fromString("2024-12-09 08:30:00", "yyyy-MM-dd HH:mm:ss"), QDateTime::fromString("2024-12-09 09:30:00", "yyyy-MM-dd HH:mm:ss"), "中国国际航空", "Delayed"}
    };


    // 准备插入语句
    QSqlQuery queryInsert;
    const QString insertStmt = "INSERT INTO flight_info (flight_number, departure_city, arrival_city, departure_time, arrival_time, price, departure_airport, arrival_airport, checkin_start_time, checkin_end_time, airline_company, status) "
                               "VALUES (:flight_number, :departure_city, :arrival_city, :departure_time, :arrival_time, :price, :departure_airport, :arrival_airport, :checkin_start_time, :checkin_end_time, :airline_company, :status)";
    if (!queryInsert.prepare(insertStmt)) {
        qDebug() << "Failed to prepare insert statement:" << queryInsert.lastError();
        QSqlDatabase::database().rollback();
        return;
    }

    for (const QVariant &flight : flights) {
        QVariantList flightList = flight.toList();
        if (flightList.size() != 12) {
            qDebug() << "Invalid flight data size:" << flightList;
            continue;
        }

        // 绑定参数
        queryInsert.bindValue(":flight_number", flightList.at(0).toString());
        queryInsert.bindValue(":departure_city", flightList.at(1).toString());
        queryInsert.bindValue(":arrival_city", flightList.at(2).toString());
        queryInsert.bindValue(":departure_time", flightList.at(3).toDateTime());
        queryInsert.bindValue(":arrival_time", flightList.at(4).toDateTime());
        queryInsert.bindValue(":price", flightList.at(5).toDouble());
        queryInsert.bindValue(":departure_airport", flightList.at(6).toString());
        queryInsert.bindValue(":arrival_airport", flightList.at(7).toString());
        queryInsert.bindValue(":checkin_start_time", flightList.at(8).toDateTime());
        queryInsert.bindValue(":checkin_end_time", flightList.at(9).toDateTime());
        queryInsert.bindValue(":airline_company", flightList.at(10).toString());
        queryInsert.bindValue(":status", flightList.at(11).toString());

        // 执行插入
        if (!queryInsert.exec()) {
            qDebug() << "Failed to insert flight:" << flightList.at(0).toString() << " Error:" << queryInsert.lastError();
            QSqlDatabase::database().rollback();
            return;
        }
        // 调用 finish() 确保资源被正确释放
        queryInsert.finish();
    }

    // 提交事务
    if (!QSqlDatabase::database().commit()) {
        qDebug() << "Failed to commit transaction";
        return;
    }

    qDebug() << "Sample flights population completed.";
}


DatabaseManager::~DatabaseManager() {
    // 关闭数据库连接
    if(m_db.isOpen()) {
        m_db.close();
    }
}
