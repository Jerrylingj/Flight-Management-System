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
    QString flightNumber;
    QString departure;
    QString destination;
    QDateTime departureTime;
    QDateTime arrivalTime;
    QString departureAirport;
    QString arrivalAirport;
    QDateTime checkInStartTime;
    QDateTime checkInEndTime;
    QString airlineCompany;
    QString status;

    QString orderDate;
    double amount;

    OrderInfo();

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

#endif // ORDER_INFO_DTO_H
