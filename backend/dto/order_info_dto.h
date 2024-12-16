#ifndef ORDER_INFO_DTO_H
#define ORDER_INFO_DTO_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>


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

#endif // ORDER_INFO_DTO_H
