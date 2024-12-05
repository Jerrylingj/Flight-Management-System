#include "FlightAPI.h"
#include "database/databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>

FlightAPI::FlightAPI(QObject *parent) : QObject(parent) {}

std::shared_ptr<ResponseDTO<QList<FlightInfo>>> FlightAPI::getAllFlights()
{
    QList<FlightInfo> flightList;
    if (!m_dbManager.connectToDatabase()) {
        return fail<QList<FlightInfo>>("Database connection failed");
    }

    QSqlQuery query("SELECT * FROM flight_info");
    while (query.next()) {
        FlightInfo flight;
        flight.flightNumber = query.value("flight_number").toString();
        flight.departureCity = query.value("departure_city").toString();
        flight.arrivalCity = query.value("arrival_city").toString();
        flight.price = query.value("price").toDouble();
        flight.departureAirport = query.value("departure_airport").toString();
        flight.arrivalAirport = query.value("arrival_airport").toString();
        flight.airlineCompany = query.value("airline_company").toString();
        flight.checkinStartTime = query.value("checkin_start_time").toDateTime();
        flight.checkinEndTime = query.value("checkin_end_time").toDateTime();
        flight.status = query.value("status").toString();
        flightList.append(flight);
    }

    if (flightList.isEmpty()) {
        return fail<QList<FlightInfo>>("No flights found", 404);
    }

    return success(flightList);
}

std::shared_ptr<ResponseDTO<QString>> FlightAPI::addFlight(const FlightInfo &flight)
{
    if (!m_dbManager.connectToDatabase()) {
        return fail<QString>("Database connection failed");
    }

    QSqlQuery query;
    query.prepare("INSERT INTO flight_info (flight_number, departure_city, arrival_city, price, "
                  "departure_airport, arrival_airport, airline_company, checkin_start_time, "
                  "checkin_end_time, status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(flight.flightNumber);
    query.addBindValue(flight.departureCity);
    query.addBindValue(flight.arrivalCity);
    query.addBindValue(flight.price);
    query.addBindValue(flight.departureAirport);
    query.addBindValue(flight.arrivalAirport);
    query.addBindValue(flight.airlineCompany);
    query.addBindValue(flight.checkinStartTime);
    query.addBindValue(flight.checkinEndTime);
    query.addBindValue(flight.status);

    if (!query.exec()) {
        return fail<QString>("Failed to add flight: " + query.lastError().text());
    }

    return success<QString>("Flight added successfully");
}

std::shared_ptr<ResponseDTO<QString>> FlightAPI::updateFlight(int flightId, const FlightInfo &flight)
{
    if (!m_dbManager.connectToDatabase()) {
        return fail<QString>("Database connection failed");
    }

    QSqlQuery query;
    query.prepare("UPDATE flight_info SET flight_number = ?, departure_city = ?, arrival_city = ?, "
                  "price = ?, departure_airport = ?, arrival_airport = ?, airline_company = ?, "
                  "checkin_start_time = ?, checkin_end_time = ?, status = ? WHERE flight_id = ?");
    query.addBindValue(flight.flightNumber);
    query.addBindValue(flight.departureCity);
    query.addBindValue(flight.arrivalCity);
    query.addBindValue(flight.price);
    query.addBindValue(flight.departureAirport);
    query.addBindValue(flight.arrivalAirport);
    query.addBindValue(flight.airlineCompany);
    query.addBindValue(flight.checkinStartTime);
    query.addBindValue(flight.checkinEndTime);
    query.addBindValue(flight.status);
    query.addBindValue(flightId);

    if (!query.exec()) {
        return fail<QString>("Failed to update flight: " + query.lastError().text());
    }

    return success<QString>("Flight updated successfully");
}

std::shared_ptr<ResponseDTO<QString>> FlightAPI::deleteFlight(int flightId)
{
    if (!m_dbManager.connectToDatabase()) {
        return fail<QString>("Database connection failed");
    }

    QSqlQuery query;
    query.prepare("DELETE FROM flight_info WHERE flight_id = ?");
    query.addBindValue(flightId);

    if (!query.exec()) {
        return fail<QString>("Failed to delete flight: " + query.lastError().text());
    }

    return success<QString>("Flight deleted successfully");
}
