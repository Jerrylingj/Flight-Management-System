#include "flight_update_dto.h"

FlightUpdateDTO::FlightUpdateDTO(const QHttpServerRequest &request) {
    QJsonDocument body = QJsonDocument::fromJson(request.body());
    QJsonObject json = body.object();

    authCode = json.value("authCode").toString();
    flightId = json.value("flightId").toInt();
    flightNumber = json.value("flightNumber").toString();
    departureCity = json.value("departureCity").toString();
    arrivalCity = json.value("arrivalCity").toString();

    // 直接将时间字段解析为 QDateTime 对象
    departureTime = QDateTime::fromString(json.value("departureTime").toString(), "ddd MMM d HH:mm:ss yyyy");
    arrivalTime = QDateTime::fromString(json.value("arrivalTime").toString(), "ddd MMM d HH:mm:ss yyyy");
    checkinStartTime = QDateTime::fromString(json.value("checkinStartTime").toString(), "ddd MMM d HH:mm:ss yyyy");
    checkinEndTime = QDateTime::fromString(json.value("checkinEndTime").toString(), "ddd MMM d HH:mm:ss yyyy");

    if (!departureTime.isValid() || !arrivalTime.isValid() || !checkinStartTime.isValid() || !checkinEndTime.isValid()) {
        throw std::invalid_argument("Invalid date-time format.");
    }

    price = json.value("price").toDouble();
    departureAirport = json.value("departureAirport").toString();
    arrivalAirport = json.value("arrivalAirport").toString();
    airlineCompany = json.value("airlineCompany").toString();
    status = json.value("status").toString();
}

bool FlightUpdateDTO::isValid() const {
    return !authCode.isEmpty() &&
           flightId > 0 &&
           !flightNumber.isEmpty() &&
           !departureCity.isEmpty() &&
           !arrivalCity.isEmpty() &&
           departureTime.isValid() &&
           arrivalTime.isValid() &&
           price > 0 &&
           !departureAirport.isEmpty() &&
           !arrivalAirport.isEmpty() &&
           !airlineCompany.isEmpty() &&
           checkinStartTime.isValid() &&
           checkinEndTime.isValid() &&
           !status.isEmpty();
}
