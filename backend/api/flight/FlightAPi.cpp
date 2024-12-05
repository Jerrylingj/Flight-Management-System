#include "FlightAPI.h"
#include "FlightInfo.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

FlightAPI::FlightAPI(QObject *parent) : QObject(parent) {
    // 在构造函数中可以执行一些初始化工作
    // 例如连接数据库
    if (!m_dbManager.connectToDatabase()) {
        qDebug() << "无法连接数据库！";
    } else {
        qDebug() << "数据库连接成功！";
    }
}

FlightAPI::~FlightAPI() {
    // 析构时不需要手动断开数据库，DatabaseManager 会自动关闭
}

// 获取所有航班信息
QList<FlightInfo> FlightAPI::getAllFlights() {
    QList<FlightInfo> flights;

    // 这里可以进行航班信息的查询，假设你的数据库中有一个航班表 "flights"
    QSqlQuery query("SELECT * FROM flights");

    if (!query.exec()) {
        qDebug() << "查询失败:" << query.lastError().text();
    } else {
        while (query.next()) {
            FlightInfo flight;
            flight.flightId = query.value("flight_id").toInt();
            flight.flightNumber = query.value("flight_number").toString();
            flight.departureCity = query.value("departure_city").toString();
            flight.arrivalCity = query.value("arrival_city").toString();
            flight.departureAirport = query.value("departure_airport").toString();
            flight.arrivalAirport = query.value("arrival_airport").toString();
            flight.checkinStartTime = query.value("checkin_start_time").toDateTime();
            flight.checkinEndTime = query.value("checkin_end_time").toDateTime();
            flight.price = query.value("price").toDouble();
            flight.airlineCompany = query.value("airline_company").toString();
            flight.status = query.value("status").toString();

            flights.append(flight);
        }
    }

    return flights;
}

// 根据航班ID获取航班信息
FlightInfo FlightAPI::getFlightById(int flightId) {
    FlightInfo flight;

    QSqlQuery query;
    query.prepare("SELECT * FROM flights WHERE flight_id = :flight_id");
    query.bindValue(":flight_id", flightId);

    if (!query.exec()) {
        qDebug() << "查询失败:" << query.lastError().text();
        return flight;  // 返回空的FlightInfo
    }

    if (query.next()) {
        flight.flightId = query.value("flight_id").toInt();
        flight.flightNumber = query.value("flight_number").toString();
        flight.departureCity = query.value("departure_city").toString();
        flight.arrivalCity = query.value("arrival_city").toString();
        flight.departureAirport = query.value("departure_airport").toString();
        flight.arrivalAirport = query.value("arrival_airport").toString();
        flight.checkinStartTime = query.value("checkin_start_time").toDateTime();
        flight.checkinEndTime = query.value("checkin_end_time").toDateTime();
        flight.price = query.value("price").toDouble();
        flight.airlineCompany = query.value("airline_company").toString();
        flight.status = query.value("status").toString();
    }

    return flight;
}
