#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager {
public:
    DatabaseManager();

    bool connectToDatabase();

    void createTable();

    void insertUser(const QString& username, const QString& telephone);

    void queryUsers();

    ~DatabaseManager();

private:
    QSqlDatabase m_db;
};


#endif // DATABASEMANAGER_H
