#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFileInfo>

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
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INT AUTO_INCREMENT PRIMARY KEY, "
               "username VARCHAR(50), "
               "telephone VARCHAR(100))");

    if(query.lastError().isValid()) {
        qDebug() << "Create table error:" << query.lastError().text();
    }
}

void DatabaseManager::insertUser(const QString& username, const QString& telephone) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, telephone) VALUES (:username, :telephone)");
    query.bindValue(":username", username);
    query.bindValue(":telephone", telephone);

    if(!query.exec()) {
        qDebug() << "Insert error:" << query.lastError().text();
    }
}

void DatabaseManager::queryUsers() {
    QSqlQuery query("SELECT * FROM users");
    while(query.next()) {
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString telephone = query.value(2).toString();
        qDebug() << "User:" << id << username << telephone;
    }
}

DatabaseManager::~DatabaseManager() {
    // 关闭数据库连接
    if(m_db.isOpen()) {
        m_db.close();
    }
}
