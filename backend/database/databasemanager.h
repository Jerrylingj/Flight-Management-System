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

    // 插入用户，传入的密码应该是原文
    bool insertUser(const QString& username, const QString& telephone, const QString& password);
    // 插入航班信息
    bool insertFlight(const QString& flightNumber, const QString& departureCity, const QString& arrivalCity,
                      double price, const QString& departureAirport, const QString& arrivalAirport,
                      const QString& airlineCompany, const QDateTime& checkinStartTime,
                      const QDateTime& checkinEndTime, const QString& status);

    // 根据手机号找用户，存在返回true，不存在返回false
    bool queryUsers(const QString& telephone);
    // 根据手机号和密码找用户，存在返回true，不存在返回false
    bool queryUsers(const QString& telephone, const QString& password);


    /*** 测试函数 ***/
    bool isFlightInfoEmpty() const; // 判断航班表是否为空
    void populateSampleFlights();   // 插入测试航班数据

    ~DatabaseManager();

private:
    QSqlDatabase m_db;
};


#endif // DATABASEMANAGER_H
