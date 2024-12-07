#include "FlightApi.h"
#include "dto/response_dto.h"
#include "dto/flight_info_dto.h"
#include <QVariant>
#include <QDebug>
#include <QJsonArray>

<<<<<<< HEAD
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
            flight.departureTime = query.value("departure_time").toDateTime();
            flight.arrivalTime = query.value("arrival_time").toDateTime();
            flight.departureAirport = query.value("departure_airport").toString();
            flight.arrivalAirport = query.value("arrival_airport").toString();
            flight.checkinStartTime = query.value("checkin_start_time").toDateTime();
            flight.checkinEndTime = query.value("checkin_end_time").toDateTime();
            flight.price = query.value("price").toDouble();
            flight.airlineCompany = query.value("airline_company").toString();
            flight.status = query.value("status").toString();

            flights.append(flight);
        }
=======
QJsonObject getAllFlights(DatabaseManager* m_db){
    try{
        QJsonArray flights;
        m_db->queryFlight(flights);
        auto response = success(flights);
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("失败");
        return response->toJson();
>>>>>>> 5aa9790 (修改代码)
    }
}

QJsonObject getFlightByID(int flightID,DatabaseManager* m_db){
    try{
        FlightInfo flight;
        m_db->queryFlight(flightID,flight);
        auto response = success(flight.toJson());
        return response->toJson();
    }catch(const std::invalid_argument& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("失败");
        return response->toJson();
    }
<<<<<<< HEAD

    if (query.next()) {
        flight.flightId = query.value("flight_id").toInt();
        flight.flightNumber = query.value("flight_number").toString();
        flight.departureCity = query.value("departure_city").toString();
        flight.arrivalCity = query.value("arrival_city").toString();
        flight.departureTime = query.value("departure_time").toDateTime();
        flight.arrivalTime = query.value("arrival_time").toDateTime();
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
=======
>>>>>>> 5aa9790 (修改代码)
}
