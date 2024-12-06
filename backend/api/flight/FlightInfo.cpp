#include "FlightInfo.h"
#include <QJsonObject>
#include <QJsonValue>

FlightInfo::FlightInfo() : flightId(0), price(0.0) {
    // 默认构造函数
    qDebug() << "创建了一个默认的 FlightInfo 对象。";
}

FlightInfo::FlightInfo(int id, const QString &flightNum, const QString &depCity, const QString &arrCity,const QDateTime &depTime, const QDateTime &arrTime,
                       const QString &depAirport, const QString &arrAirport, const QDateTime &checkinStart,
                       const QDateTime &checkinEnd, double pr, const QString &airline, const QString &status)
    : flightId(id), flightNumber(flightNum), departureCity(depCity), arrivalCity(arrCity), departureTime(depTime), arrivalTime(arrTime),
    departureAirport(depAirport), arrivalAirport(arrAirport), checkinStartTime(checkinStart),
    checkinEndTime(checkinEnd), price(pr), airlineCompany(airline), status(status)
{
    // 中文调试输出所有字段的值
    qDebug() << "创建了一个 FlightInfo 对象，字段信息如下：";
    qDebug() << "航班 ID:" << flightId;
    qDebug() << "航班号:" << flightNumber;
    qDebug() << "起点城市:" << departureCity;
    qDebug() << "终点城市:" << arrivalCity;
    qDebug() << "出发时间:" << departureTime;
    qDebug() << "到达时间:" << arrivalTime;
    qDebug() << "起点机场:" << departureAirport;
    qDebug() << "终点机场:" << arrivalAirport;
    qDebug() << "检票开始时间:" << checkinStartTime.toString();
    qDebug() << "检票结束时间:" << checkinEndTime.toString();
    qDebug() << "票价:" << price;
    qDebug() << "航空公司:" << airlineCompany;
    qDebug() << "航班状态:" << status;
}

QJsonObject FlightInfo::toJson() const
{
    QJsonObject json;
    json["flightId"] = flightId;
    json["flightNumber"] = flightNumber;
    json["departureCity"] = departureCity;
    json["arrivalCity"] = arrivalCity;
    json["departureTime"] = departureTime.toString();
    json["arrivalTime"] = arrivalTime.toString();
    json["departureAirport"] = departureAirport;
    json["arrivalAirport"] = arrivalAirport;
    json["checkinStartTime"] = checkinStartTime.toString();
    json["checkinEndTime"] = checkinEndTime.toString();
    json["price"] = price;
    json["airlineCompany"] = airlineCompany;
    json["status"] = status;

    // 调试输出转换后的 JSON 数据
    qDebug() << "FlightInfo 转换为 JSON 数据：" << json;
    return json;
}
