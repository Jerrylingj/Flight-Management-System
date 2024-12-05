#ifndef FLIGHTINFO_H
#define FLIGHTINFO_H

#include <QString>
#include <QDateTime>

struct FlightInfo {
    QString flightNumber;         // 航班号
    QString departureCity;        // 起点城市
    QString arrivalCity;          // 终点城市
    double price;                 // 票价
    QString departureAirport;     // 起点机场
    QString arrivalAirport;       // 终点机场
    QString airlineCompany;       // 航空公司
    QDateTime checkinStartTime;   // 检票开始时间
    QDateTime checkinEndTime;     // 检票结束时间
    QString status;               // 航班状态

    // 构造函数
    FlightInfo() = default;

    FlightInfo(const QString &flightNumber, const QString &departureCity, const QString &arrivalCity,
               double price, const QString &departureAirport, const QString &arrivalAirport,
               const QString &airlineCompany, const QDateTime &checkinStartTime,
               const QDateTime &checkinEndTime, const QString &status)
        : flightNumber(flightNumber), departureCity(departureCity), arrivalCity(arrivalCity),
        price(price), departureAirport(departureAirport), arrivalAirport(arrivalAirport),
        airlineCompany(airlineCompany), checkinStartTime(checkinStartTime),
        checkinEndTime(checkinEndTime), status(status) {}
};

#endif // FLIGHTINFO_H
