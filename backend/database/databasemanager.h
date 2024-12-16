#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include "dto/flight_info_dto.h"
#include "dto/order_info_dto.h"

class DatabaseManager {
public:
    DatabaseManager();

    // 连接数据库，只连一次就好
    bool connectToDatabase();

    // 自动创建表
    void createTable();

    /*** 对各个表的操作 ***/
    /*** user表 ***/
    // 插入用户，传入的密码应该是原文
    bool insertUser(const QString& username, const QString& telephone, const QString& password);
    // 根据邮箱找用户，存在返回true，不存在返回false
    bool queryUser(const QString& email);
    // 根据邮箱和密码找用户，存在返回用户id，不存在返回-1
    int queryUser(const QString& email, const QString& password);
    void queryUser(const int userId, QJsonObject& userInfo);
    // 获取余额
    double getUserBalance(int userID);

    /*** flight_info表 ***/
    // 插入航班信息
    bool insertFlight(const QString& flightNumber, const QString& departureCity, const QString& arrivalCity,const QDateTime& departureTime, const QDateTime& arrivalTime,
                      double price, const QString& departureAirport, const QString& arrivalAirport,
                      const QString& airlineCompany, const QDateTime& checkinStartTime,
                      const QDateTime& checkinEndTime, const QString& status);
    // 查询航班：查询全部航班或特定ID航班
    void queryFlight(int flightID,FlightInfo& flight);
    void queryFlight(QJsonArray& flights);
    void queryFlight(QJsonArray& flights, QString departureCity, QString arrivalCity);

    /*** orders ***/
    // void createOrder(int userID, int flightID);
    // void queryOrder(int orderId, OrderInfo& order);
    void queryOrder(QJsonArray& orders, int userId);

    /*** flight_favorites ***/
    // 收藏
    bool addFavorite(int userId, int flightId);
    // 取消收藏
    bool removeFavorite(int userId, int flightId);
    // 查询我的收藏
    QList<int> queryFavorites(int userId);

    /*** 测试函数 ***/
    /*** flight_info相关 ***/
    bool isFlightInfoEmpty() const; // 判断航班表是否为空
    void populateSampleFlights();   // 插入测试航班数据
    void populateSampleOrders();    // 插入测试订单数据
    ~DatabaseManager();

private:
    QSqlDatabase m_db;
};


#endif // DATABASEMANAGER_H
