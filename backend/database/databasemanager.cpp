#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QString>
#include <stdexcept>
#include "util/easycrypt.h"

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
                    "email VARCHAR(225) NOT NULL, "
                    "password VARCHAR(100) NOT NULL, "
                    "avatar_url VARCHAR(255), "
                    "balance DECIMAL(10, 2) DEFAULT 0.00, "
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)")){
        qDebug() << "create users error: " << query.lastError().text();
    }

    // 航班信息
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS flight_info ("
            "flight_id INTEGER PRIMARY KEY AUTO_INCREMENT, "   // 航班ID，自增主键
            "flight_number VARCHAR(10) NOT NULL, "             // 航班号
            "departure_city VARCHAR(20) NOT NULL, "            // 起点城市
            "arrival_city VARCHAR(20) NOT NULL, "              // 终点城市
            "departure_time DATETIME NOT NULL, "               // 出发时间
            "arrival_time DATETIME NOT NULL, "                 // 到达时间
            "price DECIMAL(10,2) DEFAULT 700.00, "             // 票价（最多10位数字，两位小数）
            "departure_airport VARCHAR(20) NOT NULL, "         // 起点机场
            "arrival_airport VARCHAR(20) NOT NULL, "           // 终点机场
            "airline_company VARCHAR(20) NOT NULL, "           // 航空公司
            "checkin_start_time DATETIME NOT NULL, "           // 检票开始时间
            "checkin_end_time DATETIME NOT NULL, "             // 检票结束时间
            "status VARCHAR(10) NOT NULL"
            ");")) {                                           // 航班状态
        qDebug() << "create flight_info error: " << query.lastError().text();
    }

    // 所有订单
    if(!query.exec("CREATE TABLE IF NOT EXISTS order_info ("
                    "id INT AUTO_INCREMENT PRIMARY KEY, "               // 订单ID
                    "user_id INT NOT NULL, "                            // 用户ID
                    "flight_id INT NOT NULL, "                          // 航班ID
                    "total_change_count INT NOT NULL, "                 // 总改签次数
                    "payment_status BOOLEAN NOT NULL, "                 // 支付状态
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
bool DatabaseManager::insertUser(const QString& username, const QString& email, const QString& password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, email, password) VALUES (:username, :email, :password)");
    QString hashedPassword = hashText(password);
    query.bindValue(":username", username);
    query.bindValue(":email", email);
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
bool DatabaseManager::queryUser(const QString& email){
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE email = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}
int DatabaseManager::queryUser(const QString& email, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE email = :email AND password = :pwd");
    query.bindValue(":email", email);
    QString hashedPassword = hashText(password);
    query.bindValue(":pwd", hashedPassword);

    if (query.exec() && query.next()) {
        int id = query.value("id").toInt();
        return id;
    }

    return -1;
}

void DatabaseManager::queryUser(const int userId, QJsonObject& userInfo){
    QSqlQuery query;
    query.prepare("SELECT username, email, avatar_url, balance, created_at FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    if(query.exec() && query.next()){
        // 正确的字段对应关系
        userInfo.insert("username", query.value("username").toString());
        userInfo.insert("email", query.value("email").toString());
        userInfo.insert("avatar_url", query.value("avatar_url").toString());
        userInfo.insert("balance", query.value("balance").toDouble());
        userInfo.insert("created_at", query.value("created_at").toString());
        return;
    }
    throw std::runtime_error("error!");
}

void DatabaseManager::putUser(const int userId, const double balance){
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    double bal;
    if(query.exec() && query.next()){
        bal = query.value("balance").toDouble();
    }else{
        throw std::runtime_error("error!");
    }
    QSqlQuery newQuery;
    const double newBal = bal + balance;
    newQuery.prepare("UPDATE users SET balance = :balance WHERE id = :id");
    newQuery.bindValue(":balance", newBal);
    newQuery.bindValue(":id", userId);

    if (!newQuery.exec()) {
        throw std::runtime_error("Error updating user balance!");
    }
}

/// 感觉在这用枚举变量有点怪，可能文件夹设置的还是不好吧
/// 如果mode为0，说明要修改用户名
/// 如果mode为1，说明要修改头像地址
void DatabaseManager::putUser(const int userId, const int mode, const QString str){
    QSqlQuery query;
    switch(mode){
    case 0:{
        query.prepare("UPDATE users SET username = :username WHERE id = :id");
        query.bindValue(":id", userId);
        query.bindValue(":username",str);
        break;
    }
    case 1:{
        query.prepare("UPDATE users SET avatar_url = :avatar_url WHERE id = :id");
        query.bindValue(":id", userId);
        query.bindValue(":avatar_url",str);
        break;
    }
    default:{
        throw std::invalid_argument("Mode out of range: " + std::to_string(mode));
    }
    }
    if(!query.exec()){
        throw std::runtime_error("SQL Error: " + query.lastError().text().toStdString());
    }
}

void DatabaseManager::deleteUser(const int userId) {
    // 获取数据库连接
    QSqlDatabase db = QSqlDatabase::database();

    // 检查数据库连接
    if (!db.isOpen()) {
        throw std::runtime_error("Database is not open");
    }

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    // 开始事务
    db.transaction();

    try {
        if (!query.exec()) {
            // 执行失败，回滚事务
            db.rollback();
            throw std::runtime_error("SQL Error: " + query.lastError().text().toStdString() +
                                     ", Query: " + query.lastQuery().toStdString());
        }

        // 提交事务
        db.commit();
    } catch (const std::exception& e) {
        // 捕获异常并回滚事务
        db.rollback();
        throw;  // 重新抛出异常
    }
}

void DatabaseManager::queryAllUserList(QJsonArray& arr){
    QString sql = "SELECT * FROM users";
    qDebug() << "Executing SQL:" << sql;
    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "查询失败:" << query.lastError().text();
        throw std::runtime_error("查询失败");
    } else {
        while (query.next()) {
            QJsonObject userinfo;
            userinfo["name"] = query.value("username").toString();
            userinfo["avatar"] = query.value("avatar_url").toString();
            userinfo["balance"] = query.value("balance").toDouble();
            userinfo["email"] = query.value("email").toString();
            userinfo["password"] = query.value("password").toString();
            arr.append(userinfo);
        }
    }

    qDebug() << "Retrieved" << arr.size() << "flights.";
}


/*** flight_info ***/
// 添加航班
bool DatabaseManager::insertFlight(const QString& flightNumber, const QString& departureCity, const QString& arrivalCity,const QDateTime& departureTime, const QDateTime& arrivalTime, double price, const QString& departureAirport, const QString& arrivalAirport,const QString& airlineCompany, const QDateTime& checkinStartTime, const QDateTime& checkinEndTime, const QString& status) {
    // 调用字段校验函数
    if (!validateFlightInput(flightNumber, departureCity, arrivalCity,
                             departureTime, arrivalTime, price,
                             departureAirport, arrivalAirport, airlineCompany,
                             checkinStartTime, checkinEndTime, status)) {
        qDebug() << "航班信息有误！！";
        return false;
    }

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
// 动态查询航班
void DatabaseManager::queryFlight(QJsonArray &flights, int offset, int limit)
{
    QString sql = QString("SELECT * FROM flight_info LIMIT %1 OFFSET %2").arg(limit).arg(offset);
    qDebug() << "Executing SQL:" << sql;

    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "查询失败:" << query.lastError().text();
        throw std::runtime_error("查询失败");
    }

    while (query.next()) {
        FlightInfo flight;

        flight.flightId = query.value("flight_id").isNull() ? 0 : query.value("flight_id").toInt();
        flight.flightNumber = query.value("flight_number").isNull() ? "Unknown" : query.value("flight_number").toString();
        flight.departureCity = query.value("departure_city").isNull() ? "Unknown" : query.value("departure_city").toString();
        flight.arrivalCity = query.value("arrival_city").isNull() ? "Unknown" : query.value("arrival_city").toString();
        flight.departureTime = query.value("departure_time").isNull() ? QDateTime() : query.value("departure_time").toDateTime();
        flight.arrivalTime = query.value("arrival_time").isNull() ? QDateTime() : query.value("arrival_time").toDateTime();
        flight.departureAirport = query.value("departure_airport").isNull() ? "Unknown" : query.value("departure_airport").toString();
        flight.arrivalAirport = query.value("arrival_airport").isNull() ? "Unknown" : query.value("arrival_airport").toString();
        flight.price = query.value("price").isNull() ? 0.0 : query.value("price").toDouble();
        flight.airlineCompany = query.value("airline_company").isNull() ? "Unknown" : query.value("airline_company").toString();
        flight.status = query.value("status").isNull() ? "Unknown" : query.value("status").toString();

        flights.append(flight.toJson());
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

            // 检查每个字段是否有效，如果无效则赋默认值
            flight.flightId = query.value("flight_id").isNull() ? 0 : query.value("flight_id").toInt();
            flight.flightNumber = query.value("flight_number").isNull() ? "Unknown" : query.value("flight_number").toString();
            flight.departureCity = query.value("departure_city").isNull() ? "Unknown" : query.value("departure_city").toString();
            flight.arrivalCity = query.value("arrival_city").isNull() ? "Unknown" : query.value("arrival_city").toString();
            flight.departureTime = query.value("departure_time").isNull() ? QDateTime() : query.value("departure_time").toDateTime();
            flight.arrivalTime = query.value("arrival_time").isNull() ? QDateTime() : query.value("arrival_time").toDateTime();
            flight.departureAirport = query.value("departure_airport").isNull() ? "Unknown" : query.value("departure_airport").toString();
            flight.arrivalAirport = query.value("arrival_airport").isNull() ? "Unknown" : query.value("arrival_airport").toString();
            flight.checkinStartTime = query.value("checkin_start_time").isNull() ? QDateTime() : query.value("checkin_start_time").toDateTime();
            flight.checkinEndTime = query.value("checkin_end_time").isNull() ? QDateTime() : query.value("checkin_end_time").toDateTime();
            flight.price = query.value("price").isNull() ? 0.0 : query.value("price").toDouble();
            flight.airlineCompany = query.value("airline_company").isNull() ? "Unknown" : query.value("airline_company").toString();
            flight.status = query.value("status").isNull() ? "Unknown" : query.value("status").toString();

            // 确保航班信息有效，添加到 JSON 数组
            flights.append(flight.toJson());
        }
    }

    qDebug() << "Retrieved" << flights.size() << "flights.";
    if (flights.isEmpty()) {
        qDebug() << "没有数据";
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
// 删除航班
bool DatabaseManager::deleteFlight(int flightId) {
    QString sql = "DELETE FROM flight_info WHERE flight_id = :flight_id";
    qDebug() << "Preparing SQL:" << sql << "with flight_id =" << flightId;

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":flight_id", flightId);

    if (!query.exec()) {
        qDebug() << "删除失败:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No flight found with flight_id =" << flightId;
        return false;
    }

    qDebug() << "Flight deleted successfully with flight_id =" << flightId;
    return true;
}
// 更新航班状态
bool DatabaseManager::updateFlightInfo(int flightId,
                                       const QString& flightNumber,
                                       const QString& departureCity,
                                       const QString& arrivalCity,
                                       const QDateTime& departureTime,
                                       const QDateTime& arrivalTime,
                                       double price,
                                       const QString& departureAirport,
                                       const QString& arrivalAirport,
                                       const QString& airlineCompany,
                                       const QDateTime& checkinStartTime,
                                       const QDateTime& checkinEndTime,
                                       const QString& status) {
    // 调用字段校验函数
    if (!validateFlightInput(flightNumber, departureCity, arrivalCity,
                             departureTime, arrivalTime, price,
                             departureAirport, arrivalAirport, airlineCompany,
                             checkinStartTime, checkinEndTime, status)) {
        qDebug() << "航班信息有误！！";
        return false;
    }

    QString sql = R"(
        UPDATE flight_info
        SET flight_number = :flight_number,
            departure_city = :departure_city,
            arrival_city = :arrival_city,
            departure_time = :departure_time,
            arrival_time = :arrival_time,
            price = :price,
            departure_airport = :departure_airport,
            arrival_airport = :arrival_airport,
            airline_company = :airline_company,
            checkin_start_time = :checkin_start_time,
            checkin_end_time = :checkin_end_time,
            status = :status
        WHERE flight_id = :flight_id
    )";
    QSqlQuery query;
    query.prepare(sql);

    query.bindValue(":flight_id", flightId);
    query.bindValue(":flight_number", flightNumber);
    query.bindValue(":departure_city", departureCity);
    query.bindValue(":arrival_city", arrivalCity);
    query.bindValue(":departure_time", departureTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":arrival_time", arrivalTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":price", price);
    query.bindValue(":departure_airport", departureAirport);
    query.bindValue(":arrival_airport", arrivalAirport);
    query.bindValue(":airline_company", airlineCompany);
    query.bindValue(":checkin_start_time", checkinStartTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":checkin_end_time", checkinEndTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":status", status);

    if (!query.exec()) {
        qDebug() << "Failed to update flight info:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No flight found with flight_id =" << flightId;
        return false;
    }

    qDebug() << "Flight info updated successfully for flight_id =" << flightId;
    return true;
}



void DatabaseManager::queryNextFlights(int flightId, QJsonArray &flights) {
    // 通过 flightId 获取出发城市、到达城市和出发时间
    QString sql = "SELECT departure_city, arrival_city, departure_time FROM flight_info WHERE flight_id = :flightId";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":flightId", flightId);

    if (!query.exec() || !query.next()) {
        qWarning() << "无法找到指定的航班:" << query.lastError().text();
        throw std::runtime_error("无法找到指定的航班");
    }

    QString departureCity = query.value("departure_city").toString();
    QString arrivalCity = query.value("arrival_city").toString();
    QDateTime departureTime = query.value("departure_time").toDateTime();
    QDate baseDate = departureTime.date();
    QDate startDate = baseDate;        // 当天
    QDate endDate = baseDate.addDays(2); // 后天

    // 查询符合条件的所有航班
    QString nextFlightSql = R"(
        SELECT * FROM flight_info
        WHERE departure_city = :departureCity
          AND arrival_city = :arrivalCity
          AND departure_time > :departureTime
          AND DATE(departure_time) BETWEEN :startDate AND :endDate
        ORDER BY departure_time ASC
    )";
    QSqlQuery nextFlightQuery;
    nextFlightQuery.prepare(nextFlightSql);
    nextFlightQuery.bindValue(":departureCity", departureCity);
    nextFlightQuery.bindValue(":arrivalCity", arrivalCity);
    nextFlightQuery.bindValue(":departureTime", departureTime);
    nextFlightQuery.bindValue(":startDate", startDate.toString("yyyy-MM-dd"));
    nextFlightQuery.bindValue(":endDate", endDate.toString("yyyy-MM-dd"));

    if (!nextFlightQuery.exec()) {
        qWarning() << "查询错误:" << nextFlightQuery.lastError().text();
        return;
    }

    while (nextFlightQuery.next()) {
        QJsonObject flightObj;
        flightObj["flightId"] = nextFlightQuery.value("flight_id").toInt();
        flightObj["flightNumber"] = nextFlightQuery.value("flight_number").toString();
        flightObj["departureCity"] = nextFlightQuery.value("departure_city").toString();
        flightObj["arrivalCity"] = nextFlightQuery.value("arrival_city").toString();
        flightObj["departureTime"] = nextFlightQuery.value("departure_time").toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        flightObj["arrivalTime"] = nextFlightQuery.value("arrival_time").toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        flightObj["departureAirport"] = nextFlightQuery.value("departure_airport").toString();
        flightObj["arrivalAirport"] = nextFlightQuery.value("arrival_airport").toString();
        flightObj["checkinStartTime"] = nextFlightQuery.value("checkin_start_time").toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        flightObj["checkinEndTime"] = nextFlightQuery.value("checkin_end_time").toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        flightObj["price"] = nextFlightQuery.value("price").toDouble();
        flightObj["airlineCompany"] = nextFlightQuery.value("airline_company").toString();
        flightObj["status"] = nextFlightQuery.value("status").toString();
        flights.append(flightObj);
    }
}
/*** order ***/

// 查询一个用户所有订单信息
bool DatabaseManager::orderOfFlightIdExists(int flightId) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM order_info WHERE flight_id = :flightId");
    query.bindValue(":flightId", flightId);

    if (!query.exec()) {
        qWarning() << "Database query failed:" << query.lastError();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

void DatabaseManager::queryOrder(QJsonArray &orders, int userId) {
    QString sql = R"(
        SELECT
            o.id,
            o.user_id,
            o.flight_id,
            o.total_change_count,
            o.payment_status,
            f.flight_number,
            f.airline_company,
            f.price,
            f.status,
            f.departure_city,
            f.arrival_city,
            f.departure_time,
            f.arrival_time,
            f.departure_airport,
            f.arrival_airport,
            f.checkin_start_time,
            f.checkin_end_time
        FROM order_info o
        JOIN flight_info f ON o.flight_id = f.flight_id
        WHERE o.user_id = :user_id
    )";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":user_id", userId);

    qDebug() << "[调试] DatabaseManager::queryOrder - 执行 SQL 语句:" << sql;

    if (!query.exec()) {
        QString errorMsg = QString("[错误] DatabaseManager::queryOrder - 查询失败: %1").arg(query.lastError().text());
        qDebug() << errorMsg;
        throw std::runtime_error(errorMsg.toStdString());
    }

    while (query.next()) {
        OrderInfo order;
        // 订单信息
        order.orderId = query.value("id").toInt();
        order.userId = query.value("user_id").toInt();
        order.flightId = query.value("flight_id").toInt();
        order.totalChangeCount = query.value("total_change_count").toInt();
        order.paymentStatus = query.value("payment_status").toBool();

        // 航班信息
        order.flightNumber = query.value("flight_number").toString();
        order.airlineCompany = query.value("airline_company").toString();
        order.price = query.value("price").toDouble();
        order.flightStatus = query.value("status").toString();
        order.departure = query.value("departure_city").toString();
        order.destination = query.value("arrival_city").toString();
        order.departureTime = query.value("departure_time").toDateTime();
        order.arrivalTime = query.value("arrival_time").toDateTime();
        order.departureAirport = query.value("departure_airport").toString();
        order.arrivalAirport = query.value("arrival_airport").toString();
        order.checkInStartTime = query.value("checkin_start_time").toDateTime();
        order.checkInEndTime = query.value("checkin_end_time").toDateTime();

        orders.append(order.toJson());
    }

    qDebug() << "Retrieved" << orders.size() << "orders.";

    // if (orders.isEmpty()) {
    //     throw std::runtime_error("没有数据");
    // }
}

// 创建订单
void DatabaseManager::insertOrder(int userId, int flightId) {
    // 创建QSqlQuery对象
    QSqlQuery query;

    // 准备SQL语句
    query.prepare("INSERT INTO order_info (user_id, flight_id, total_change_count, payment_status) "
                  "VALUES (:user_id, :flight_id, :total_change_count, :payment_status)");

    // 绑定参数
    query.bindValue(":user_id", userId);
    query.bindValue(":flight_id", flightId);
    query.bindValue(":total_change_count", 0); // 初始改签次数为0
    query.bindValue(":payment_status", false); // 初始支付状态为false

    // 执行查询
    if (!query.exec()) {
        // 如果插入失败，输出错误信息
        qDebug() << "insert order_info error: " << query.lastError().text();
    } else {
        qDebug() << "Order inserted successfully!";
    }
}

// 删除订单
void DatabaseManager::deleteOrder(int orderId) {
    QString sql = "DELETE FROM order_info WHERE id = :order_id";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":order_id", orderId);

    qDebug() << "[调试] DatabaseManager::deleteOrder - 执行 SQL 语句:" << sql;

    if (!query.exec()) {
        QString errorMsg = QString("[错误] DatabaseManager::deleteOrder - 删除失败: %1").arg(query.lastError().text());
        qDebug() << errorMsg;
        throw std::runtime_error(errorMsg.toStdString());
    }

    if (query.numRowsAffected() == 0) {
        throw std::runtime_error("未找到要删除的订单");
    }

    qDebug() << "Successfully deleted order with ID:" << orderId;
}

// 修改特定订单的支付状态字段的函数
void DatabaseManager::updatePaymentStatus(int orderId, bool paymentStatus) {
    QString sql = R"(
        UPDATE order_info
        SET payment_status = :payment_status
        WHERE id = :order_id
    )";

    QSqlQuery query;
    query.prepare(sql);

    // 绑定参数值
    query.bindValue(":order_id", orderId);
    query.bindValue(":payment_status", paymentStatus);

    qDebug() << "[调试] DatabaseManager::updatePaymentStatus - 执行 SQL 语句:" << sql;
    qDebug() << "[调试] 绑定的 payment_status:" << paymentStatus;

    if (!query.exec()) {
        QString errorMsg = QString("[错误] DatabaseManager::updatePaymentStatus - 更新失败: %1").arg(query.lastError().text());
        qDebug() << errorMsg;
        throw std::runtime_error(errorMsg.toStdString());
    }

    if (query.numRowsAffected() == 0) {
        throw std::runtime_error("未找到要更新的订单");
    }

    qDebug() << "成功更新订单 ID:" << orderId << "的 payment_status";
}

// 修改特定订单的航班编号字段的函数，注意这时支付状态应设置为已支付
void DatabaseManager::updateFlightId(int orderId, int flightId) {
    QString sql = R"(
        UPDATE order_info
        SET flight_id = :flight_id
        WHERE id = :order_id
    )";

    QSqlQuery query;
    query.prepare(sql);

    // 绑定参数值
    query.bindValue(":order_id", orderId);
    query.bindValue(":flight_id", flightId);

    qDebug() << "[调试] DatabaseManager::updateFlightId - 执行 SQL 语句:" << sql;
    qDebug() << "[调试] 绑定的 flight_id:" << flightId;

    if (!query.exec()) {
        QString errorMsg = QString("[错误] DatabaseManager::updateFlightId - 更新失败: %1").arg(query.lastError().text());
        qDebug() << errorMsg;
        throw std::runtime_error(errorMsg.toStdString());
    }

    if (query.numRowsAffected() == 0) {
        throw std::runtime_error("未找到要更新的订单");
    }

    qDebug() << "成功更新订单 ID:" << orderId << "的 flight_id";
}

void DatabaseManager::getOrderDetails(int orderId, int& flightId, int& userId) {
    QString sql = "SELECT flight_id, user_id FROM order_info WHERE id = :order_id";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":order_id", orderId);

    if (!query.exec()) {
        QString errorMsg = QString("[错误] DatabaseManager::getOrderDetails - 查询失败: %1").arg(query.lastError().text());
        qDebug() << errorMsg;
        throw std::runtime_error(errorMsg.toStdString());
    }

    if (query.next()) {
        flightId = query.value("flight_id").toInt();
        userId = query.value("user_id").toInt();
    } else {
        throw std::runtime_error("未找到指定的订单");
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
QList<QJsonObject> DatabaseManager::queryFavorites(int userId) {
    QList<QJsonObject> flightDetails;

    QSqlQuery query;
    query.prepare(R"(
        SELECT f.flight_id, f.flight_number, f.departure_time, f.arrival_time,
               f.departure_city, f.arrival_city,
               f.departure_airport, f.arrival_airport, f.price,
               f.airline_company, f.status
        FROM flight_favorites AS fav
        JOIN flight_info AS f ON fav.flight_id = f.flight_id
        WHERE fav.user_id = :userId
    )");
    query.bindValue(":userId", userId);

    if (query.exec()) {
        while (query.next()) {
            QJsonObject flight;
            flight["flightId"] = query.value("flight_id").toInt();
            flight["flightNumber"] = query.value("flight_number").toString();
            flight["departureTime"] = query.value("departure_time").toString();
            flight["arrivalTime"] = query.value("arrival_time").toString();
            flight["departureCity"] = query.value("departure_city").toString();
            flight["arrivalCity"] = query.value("arrival_city").toString();
            flight["departureAirport"] = query.value("departure_airport").toString();
            flight["arrivalAirport"] = query.value("arrival_airport").toString();
            flight["price"] = query.value("price").toDouble();
            flight["airlineCompany"] = query.value("airline_company").toString();
            flight["status"] = query.value("status").toString();

            flightDetails.append(flight);
        }
    } else {
        qWarning() << "Failed to query favorites:" << query.lastError() << "Query:" << query.lastQuery();
    }

    return flightDetails;
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

bool DatabaseManager::validateFlightInput(const QString& flightNumber,
                                          const QString& departureCity,
                                          const QString& arrivalCity,
                                          const QDateTime& departureTime,
                                          const QDateTime& arrivalTime,
                                          double price,
                                          const QString& departureAirport,
                                          const QString& arrivalAirport,
                                          const QString& airlineCompany,
                                          const QDateTime& checkinStartTime,
                                          const QDateTime& checkinEndTime,
                                          const QString& status) {
    if (flightNumber.isEmpty() || departureCity.isEmpty() || arrivalCity.isEmpty() ||
        departureAirport.isEmpty() || arrivalAirport.isEmpty() || airlineCompany.isEmpty() ||
        status.isEmpty()) {
        qDebug() << "Error: One or more required fields are empty.";
        return false;
    }

    if (!departureTime.isValid() || !arrivalTime.isValid() ||
        !checkinStartTime.isValid() || !checkinEndTime.isValid()) {
        qDebug() << "Error: One or more datetime fields are invalid.";
        return false;
    }

    if (price < 0) {
        qDebug() << "Error: Price cannot be negative.";
        return false;
    }

    if (departureTime >= arrivalTime) {
        qDebug() << "Error: Departure time must be earlier than arrival time.";
        return false;
    }

    if (checkinStartTime >= checkinEndTime) {
        qDebug() << "Error: Check-in start time must be earlier than check-in end time.";
        return false;
    }

    return true;
}



DatabaseManager::~DatabaseManager() {
    // 关闭数据库连接
    if(m_db.isOpen()) {
        m_db.close();
    }
}
