#ifndef FLIGHTINFO_H
#define FLIGHTINFO_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QDebug>

class FlightInfo
{
public:
    int flightId;                   // 航班ID
    QString flightNumber;           // 航班号
    QString departureCity;          // 起点市
    QString arrivalCity;            // 终点市
    QDateTime departureTime;        // 出发时间
    QDateTime arrivalTime;          // 到达时间
    QString departureAirport;       // 起点机场
    QString arrivalAirport;         // 终点机场
    QDateTime checkinStartTime;     // 检票开始时间
    QDateTime checkinEndTime;       // 检票结束时间
    double price;                   // 价格
    QString airlineCompany;         // 航空公司
    QString status;                 // 航班状态（正常、延误……）

    // 构造函数
    FlightInfo();
    FlightInfo(int id, const QString &flightNum, const QString &depCity, const QString &arrCity,const QDateTime &depTime, const QDateTime &arrTime,
               const QString &depAirport, const QString &arrAirport, const QDateTime &checkinStart,
               const QDateTime &checkinEnd, double pr, const QString &airline, const QString &status);

    // 转换成 JSON 或其他格式等功能
    QJsonObject toJson() const;
};

#endif // FLIGHTINFO_H
