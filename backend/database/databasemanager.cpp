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
                    "flight_id INTEGER PRIMARY KEY AUTO_INCREMENT, "  // 航班ID，自增主键
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
                    "UNIQUE(flight_number) )")) {                     // 确保航班号唯一
        qDebug() << "create flight_info error: " << query.lastError().text();
    }

    // 所有订单
    if(!query.exec("CREATE TABLE IF NOT EXISTS all_order ("
                    "id INT AUTO_INCREMENT PRIMARY KEY, "
                    "user_id INT NOT NULL, "
                    "flight_id INT NOT NULL, "
                    "order_time DATETIME NOT NULL, "
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

DatabaseManager::~DatabaseManager() {
    // 关闭数据库连接
    if(m_db.isOpen()) {
        m_db.close();
    }
}
