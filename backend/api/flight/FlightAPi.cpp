#include "FlightAPI.h"
#include "FlightInfo.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

FlightAPI::FlightAPI(DatabaseManager* dbManager, QObject *parent) : QObject(parent), m_dbManager(dbManager) {
    // 这里不再连接数据库，直接使用传入的数据库连接
}

FlightAPI::~FlightAPI() {
    // 不需要手动断开数据库，DatabaseManager 会自动处理
}

// 获取所有航班信息
QList<FlightInfo> FlightAPI::getAllFlights() {
    QList<FlightInfo> flights;

    QString sql = "SELECT * FROM flight_info";
    qDebug() << "Executing SQL:" << sql;

    QSqlQuery query;
    if (!query.exec(sql)) {
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

    qDebug() << "Retrieved" << flights.size() << "flights.";
    return flights;
}

// 根据航班ID获取航班信息
FlightInfo FlightAPI::getFlightById(int flightId) {
    FlightInfo flight;

    QString sql = "SELECT * FROM flight_info WHERE flight_id = :flight_id";
    qDebug() << "Preparing SQL:" << sql << "with flight_id =" << flightId;

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":flight_id", flightId);

    if (!query.exec()) {
        qDebug() << "查询失败:" << query.lastError().text();
        return flight;  // 返回一个空的FlightInfo对象
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

        qDebug() << "Flight found:" << flight.flightNumber;
    } else {
        qDebug() << "No flight found with flight_id =" << flightId;
    }

    return flight;
}
