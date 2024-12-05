#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager {
public:
    DatabaseManager();

    // 连接数据库，只连一次就好
    bool connectToDatabase();

    // 自动创建表
    void createTable();

    // password应该是加密过的密码而不是原文
    bool insertUser(const QString& username, const QString& telephone, const QString& password);

    // 根据手机号和密码找用户，存在返回true，不存在返回false
    bool queryUsers(const QString& telephone, const QString& password);

    ~DatabaseManager();

private:
    QSqlDatabase m_db;
};


#endif // DATABASEMANAGER_H
