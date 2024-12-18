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
    if(!query.exec("CREATE TABLE IF NOT EXISTS flight_info ("
                    "flight_id INTEGER PRIMARY KEY AUTO_INCREMENT, "   // 航班ID，自增主键
                    "flight_number VARCHAR(10) NOT NULL, "             // 航班号
                    "departure_city VARCHAR(20) NOT NULL, "            // 起点城市
                    "arrival_city VARCHAR(20) NOT NULL, "              // 终点城市
                    "departure_time DATETIME NOT NULL, "               // 出发时间
                    "arrival_time DATETIME NOT NULL, "                 // 到达时间
                    "price DECIMAL(10,2) DEFAULT 700, "                   // 票价（最多10位数字，两位小数）
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




/*** flight_info ***/
// 添加航班
bool DatabaseManager::insertFlight(const QString& flightNumber, const QString& departureCity, const QString& arrivalCity,const QDateTime& departureTime, const QDateTime& arrivalTime, double price, const QString& departureAirport, const QString& arrivalAirport,const QString& airlineCompany, const QDateTime& checkinStartTime, const QDateTime& checkinEndTime, const QString& status) {
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

// 用于改签：获取和flightId所指向的航班相同出发地和目的地的、出发时间晚于flightId所指向的航班，同时时间距离最近的那一个航班
void DatabaseManager::queryNextFlight(int flightId, FlightInfo & flightInfo){
    // 通过 flightId 获取出发城市、到达城市和出发时间
    QString sql = "SELECT departure_city, arrival_city, departure_time FROM flight_info WHERE flight_id = :flightId";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":flightId", flightId);

    if (!query.exec() || !query.next()) {
        qDebug() << "无法找到指定的航班:" << query.lastError().text();
        flightInfo = FlightInfo();
        throw std::runtime_error("无法找到指定的航班");
    }

    QString departureCity = query.value("departure_city").toString();
    QString arrivalCity = query.value("arrival_city").toString();
    QDateTime departureTime = query.value("departure_time").toDateTime();

    // 查询下一趟航班
    QString nextFlightSql = "SELECT * FROM flight_info WHERE departure_city = :departureCity AND arrival_city = :arrivalCity AND departure_time > :departureTime ORDER BY departure_time ASC LIMIT 1";
    QSqlQuery nextFlightQuery;
    nextFlightQuery.prepare(nextFlightSql);
    nextFlightQuery.bindValue(":departureCity", departureCity);
    nextFlightQuery.bindValue(":arrivalCity", arrivalCity);
    nextFlightQuery.bindValue(":departureTime", departureTime);

    if (!nextFlightQuery.exec()) {
        qDebug() << "查询错误:" << nextFlightQuery.lastError().text();
        // 清空 flightInfo
        flightInfo = FlightInfo();
        return;
    }

    if (!nextFlightQuery.next()) {
        // 没有找到下一趟航班
        qDebug() << "没有找到下一趟航班";
        // 清空 flightInfo
        flightInfo = FlightInfo();
        return;
    }

    // 将查询结果填充到 flightInfo
    flightInfo.flightId = nextFlightQuery.value("flight_id").toInt();
    flightInfo.flightNumber = nextFlightQuery.value("flight_number").toString();
    flightInfo.departureCity = nextFlightQuery.value("departure_city").toString();
    flightInfo.arrivalCity = nextFlightQuery.value("arrival_city").toString();
    flightInfo.departureTime = nextFlightQuery.value("departure_time").toDateTime();
    flightInfo.arrivalTime = nextFlightQuery.value("arrival_time").toDateTime();
    flightInfo.departureAirport = nextFlightQuery.value("departure_airport").toString();
    flightInfo.arrivalAirport = nextFlightQuery.value("arrival_airport").toString();
    flightInfo.checkinStartTime = nextFlightQuery.value("checkin_start_time").toDateTime();
    flightInfo.checkinEndTime = nextFlightQuery.value("checkin_end_time").toDateTime();
    flightInfo.price = nextFlightQuery.value("price").toDouble();
    flightInfo.airlineCompany = nextFlightQuery.value("airline_company").toString();
    flightInfo.status = nextFlightQuery.value("status").toString();
}
/*** order ***/

// 查询一个用户所有订单信息
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
    // 如果需要清空表，可以取消注释以下代码
    /*
    if (!queryClear.exec("DELETE FROM order_info")) {
        qDebug() << "Failed to clear order_info table:" << queryClear.lastError();
        return;
    }
    qDebug() << "order_info 表已清空。";
    */

    // 开始事务
    if (!QSqlDatabase::database().transaction()) {
        qDebug() << "Failed to start transaction";
        return;
    }

    // 准备要插入的样本数据
    // 每个 QVariantList 包含：user_id, flight_id, total_change_count, payment_status
    QList<QVariantList> orders = {
        {1, 1, 0, true},
        {1, 2, 1, false},
        {1, 3, 2, true},
        {1, 4, 0, true},
        {1, 5, 1, false},
        {1, 6, 3, true},
        {1, 7, 1, true},
        {1, 8, 2, false},
        {1, 9, 0, true},
        {1, 10, 1, true}
    };

    // 准备插入语句
    QSqlQuery queryInsert;
    const QString insertStmt = "INSERT INTO order_info (user_id, flight_id, total_change_count, payment_status) "
                               "VALUES (:user_id, :flight_id, :total_change_count, :payment_status)";
    if (!queryInsert.prepare(insertStmt)) {
        qDebug() << "Failed to prepare insert statement:" << queryInsert.lastError();
        QSqlDatabase::database().rollback();
        return;
    }

    for (const QVariant &order : orders) {
        QVariantList orderList = order.toList();
        if (orderList.size() != 4) {
            qDebug() << "Invalid order data size:" << orderList;
            continue;
        }

        // 绑定参数
        queryInsert.bindValue(":user_id", orderList.at(0).toInt());
        queryInsert.bindValue(":flight_id", orderList.at(1).toInt());
        queryInsert.bindValue(":total_change_count", orderList.at(2).toInt());
        queryInsert.bindValue(":payment_status", orderList.at(3).toBool());

        // 执行插入
        if (!queryInsert.exec()) {
            qDebug() << "Failed to insert order for user_id:" << orderList.at(0).toInt()
            << " flight_id:" << orderList.at(1).toInt()
            << " Error:" << queryInsert.lastError();
            QSqlDatabase::database().rollback();
            return;
        }
        // 重置查询以准备下一次绑定
        queryInsert.finish();
        queryInsert.prepare(insertStmt);
    }

    // 提交事务
    if (!QSqlDatabase::database().commit()) {
        qDebug() << "Failed to commit transaction:" << QSqlDatabase::database().lastError();
        return;
    }

    qDebug() << "已成功生成模拟的订单数据.";
}


DatabaseManager::~DatabaseManager() {
    // 关闭数据库连接
    if(m_db.isOpen()) {
        m_db.close();
    }
}
