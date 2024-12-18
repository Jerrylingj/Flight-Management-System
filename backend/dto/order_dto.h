#ifndef ORDER_DTO_H
#define ORDER_DTO_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>

// 订单信息
class OrderInfo {
public:
    int orderId;
    int userId;
    int flightId;
    int totalChangeCount;
    bool paymentStatus;

    QString flightNumber;
    QString airlineCompany;
    double price;
    QString flightStatus;


    QString departure;
    QString destination;
    QDateTime departureTime;
    QDateTime arrivalTime;
    QString departureAirport;
    QString arrivalAirport;

    QDateTime checkInStartTime;
    QDateTime checkInEndTime;


    OrderInfo();

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

// 创建订单
class CreateOrderRequest
{
public:
    CreateOrderRequest(QJsonObject& obj);
    int getFlightID() const {
        return flightID;
    }
private:
    int flightID;
};




#endif // ORDER_DTO_H
