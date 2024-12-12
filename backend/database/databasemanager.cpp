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
    if(!query.exec("CREATE TABLE IF NOT EXISTS order_info ("
                    "id INT AUTO_INCREMENT PRIMARY KEY, "               // 订单ID
                    "user_id INT NOT NULL, "                            // 用户ID
                    "flight_id INT NOT NULL, "                          // 航班ID
                    "order_time DATETIME DEFAULT CURRENT_TIMESTAMP, "   // 订单创建时间
                    "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE, "
                    "FOREIGN KEY (flight_id) REFERENCES flight_info(flight_id) ON DELETE CASCADE)")){
        qDebug() << "create order_info error: " << query.lastError().text();
    }

    // 收藏
    if (!query.exec("CREATE TABLE IF NOT EXISTS flight_favorites ("
                    "favorite_id INTEGER PRIMARY KEY AUTO_INCREMENT, "  // 收藏ID
                    "user_id INTEGER NOT NULL, "                        // 用户ID
                    "flight_id INTEGER NOT NULL, "                      // 航班ID
                    "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE, "
                    "FOREIGN KEY (flight_id) REFERENCES flight_info(flight_id) ON DELETE CASCADE)")) {
        qDebug() << "create flight_favorites error: " << query.lastError().text();
    }
}

/*** 表的增删改查 ***/
/*** users ***/
// 添加用户
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
// 获取余额
double DatabaseManager::getUserBalance(int userID){
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE id = :userid");
    query.bindValue(":userid", userID);
    if(query.exec() && query.next()){
        return query.value("balance").toDouble();
    }
    throw std::runtime_error("无法获取用户余额");
}
// 查询用户
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

/*** flight_info ***/
// 添加航班
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
// 查询航班
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
        flight.departureTime = query.value("departure_time").toDateTime();
        flight.arrivalTime = query.value("arrival_time").toDateTime();
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
            flight.departureTime = query.value("departure_time").toDateTime();
            flight.arrivalTime = query.value("arrival_time").toDateTime();
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
void DatabaseManager::queryFlight(QJsonArray& flights, QString departureCity, QString arrivalCity){
    QString sql = "SELECT * FROM flight_info WHERE departure_city = :departureCity AND arrival_city = :arrivalCity";
    qDebug() << "Executing SQL:" << sql;

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":departureCity",departureCity);
    query.bindValue(":arrivalCity",arrivalCity);
    if (!query.exec()) {
        qDebug() << "查询失败:" << query.lastError().text();
        throw std::runtime_error("查询失败");
    } else {
        while (query.next()) {
            FlightInfo flight;
            flight.flightId = query.value("flight_id").toInt();
            flight.flightNumber = query.value("flight_number").toString();
            flight.departureCity = query.value("departure_city").toString();
            flight.arrivalCity = query.value("arrival_city").toString();
            flight.departureTime = query.value("departure_time").toDateTime();
            flight.arrivalTime = query.value("arrival_time").toDateTime();
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

/*** order ***/
// 创建订单
// void DatabaseManager::createOrder(int userID, int flightID){
//     // 如果要一次执行多次数据库操作需要这样子做
//     QSqlDatabase db = QSqlDatabase::database();
//     if(!db.transaction()){
//         throw std::runtime_error("无法开始事务");
//     }
//     try{
//         double balance = getUserBalance(userID);
//         FlightInfo flight;
//         queryFlight(flightID,flight);
//         if(flight.price > balance) {
//             throw std::invalid_argument("没钱了");
//         }
//         QSqlQuery query;
//         query.prepare("INSERT INTO order_info (user_id, flight_id) "
//                       "VALUES (:userid, :flightid) ");
//         query.bindValue(":userid",userID);
//         query.bindValue(":flightid",flightID);
//         if(!query.exec()){
//             throw std::runtime_error("插入失败");
//         }
//         double newBalance = balance - flight.price;
//         QSqlQuery updateQuery;
//         updateQuery.prepare("UPDATE users SET balance = :balance WHERE id = :userid");
//         updateQuery.bindValue(":balance", newBalance);
//         updateQuery.bindValue(":userid", userID);
//         if(!updateQuery.exec()){
//             throw std::runtime_error("更新用户余额失败");
//         }
//         // 提交事件
//         if(!db.commit()){
//             throw std::runtime_error("事务提交失败");
//         }
//     }catch(const std::exception& e){
//         // 发生错误就回滚数据库，避免票没订但是钱扣了
//         db.rollback();
//         throw;
//     }
// }

void DatabaseManager :: queryOrder(QJsonArray & orders){
    QString sql = "SELECT * FROM order_info";
    qDebug() << "Executing SQL:" << sql;

    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "查询失败:" << query.lastError().text();
        throw std::runtime_error("查询失败");
    } else {
        while (query.next()) {
            OrderInfo order;
            order.flightId = query.value("flight_id").toInt();
            order.orderId = query.value("id").toInt();
            order.userId = query.value("user_id").toInt();
            order.createdAt = query.value("order_time").toDateTime();
            FlightInfo flight;
            queryFlight(order.flightId,flight);
            order.flightNumber = flight.flightNumber;
            order.departure = flight.departureCity;
            order.departureAirport = flight.departureAirport;
            order.departureTime = flight.departureTime;
            order.destination = flight.arrivalCity;
            order.arrivalAirport = flight.arrivalAirport;
            order.arrivalTime = flight.arrivalTime;
            order.checkInStartTime = flight.checkinStartTime;
            order.checkInEndTime = flight.checkinEndTime;
            order.airlineCompany = flight.airlineCompany;
            order.status = flight.status;
            orders.append(order.toJson());
        }
    }

    qDebug() << "Retrieved" << orders.size() << "orders.";
    if(orders.isEmpty()){
        throw std::runtime_error("没有数据");
    }
}

/*** flight_favorites ***/
// 添加收藏
bool DatabaseManager::addFavorite(int userId, int flightId) {
    QSqlQuery query;
    query.prepare("INSERT INTO flight_favorites (user_id, flight_id) VALUES (:user_id, :flight_id)");
    query.bindValue(":user_id", userId);
    query.bindValue(":flight_id", flightId);

    if (!query.exec()) {
        qDebug() << "Error adding favorite:" << query.lastError().text();
        return false;
    }
    return true;
}

// 取消收藏
bool DatabaseManager::removeFavorite(int userId, int flightId) {
    QSqlQuery query;
    query.prepare("DELETE FROM flight_favorites WHERE user_id = :user_id AND flight_id = :flight_id");
    query.bindValue(":user_id", userId);
    query.bindValue(":flight_id", flightId);

    if (!query.exec()) {
        qDebug() << "Error removing favorite:" << query.lastError().text();
        return false;
    }
    return true;
}

// 查询我的收藏
QList<int> DatabaseManager::queryFavorites(int userId) {
    QSqlQuery query;
    query.prepare("SELECT flight_id FROM flight_favorites WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    QList<int> favoriteList;
    if (query.exec()) {
        while (query.next()) {
            favoriteList.append(query.value("flight_id").toInt());
        }
    } else {
        qDebug() << "Error querying favorites:" << query.lastError().text();
    }
    return favoriteList;
}


/*** 测试函数 ***/
/*** flight_info ***/
// 判断是否空
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
// 插入样例航班
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

    // 所有订单
    // if(!query.exec("CREATE TABLE IF NOT EXISTS order_info ("
    //                 "id INT AUTO_INCREMENT PRIMARY KEY, "               // 订单ID
    //                 "user_id INT NOT NULL, "                            // 用户ID
    //                 "flight_id INT NOT NULL, "                          // 航班ID
    //                 "order_time DATETIME DEFAULT CURRENT_TIMESTAMP, "   // 订单创建时间
    //                 "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE, "
    //                 "FOREIGN KEY (flight_id) REFERENCES flight_info(flight_id) ON DELETE CASCADE)")){
    //     qDebug() << "create order_info error: " << query.lastError().text();
    // }

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

void DatabaseManager::populateSampleOrders(){
    QSqlQuery queryClear;
    // if (!queryClear.exec("DELETE FROM order_info")) {
    //     qDebug() << "Failed to clear order_info table:" << queryClear.lastError();
    //     return;
    // }
    // qDebug() << "order_info 表已清空。";

    // 开始事务
    if (!QSqlDatabase::database().transaction()) {
        qDebug() << "Failed to start transaction";
        return;
    }

    QList<QVariantList> orders = {
        {1, 1, QDateTime::fromString("2024-12-01 08:00:00", "yyyy-MM-dd HH:mm:ss")},
        {2, 2, QDateTime::fromString("2024-12-01 09:00:00", "yyyy-MM-dd HH:mm:ss")},
        {3, 3, QDateTime::fromString("2024-12-01 10:00:00", "yyyy-MM-dd HH:mm:ss")},
        {4, 4, QDateTime::fromString("2024-12-01 11:00:00", "yyyy-MM-dd HH:mm:ss")},
        {1, 5, QDateTime::fromString("2024-12-01 12:00:00", "yyyy-MM-dd HH:mm:ss")},
    };

    // 准备插入语句
    QSqlQuery queryInsert;
    const QString insertStmt = "INSERT INTO order_info (user_id, flight_id, order_time) "
                               "VALUES (:user_id, :flight_id, :order_time)";
    if (!queryInsert.prepare(insertStmt)) {
        qDebug() << "Failed to prepare insert statement:" << queryInsert.lastError();
        QSqlDatabase::database().rollback();
        return;
    }

    for (const QVariant &order : orders) {
        QVariantList orderList = order.toList();
        if (orderList.size() != 3) {
            qDebug() << "Invalid order data size:" << orderList;
            continue;
        }

        // 绑定参数
        queryInsert.bindValue(":user_id", orderList.at(0).toInt());
        queryInsert.bindValue(":flight_id", orderList.at(1).toInt());
        queryInsert.bindValue(":order_time", orderList.at(2).toDateTime());

        // 执行插入
        if (!queryInsert.exec()) {
            qDebug() << "Failed to insert order:" << orderList.at(0).toInt() << " Error:" << queryInsert.lastError();
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

    qDebug() << "Sample orders population completed.";
}


DatabaseManager::~DatabaseManager() {
    // 关闭数据库连接
    if(m_db.isOpen()) {
        m_db.close();
    }
}
