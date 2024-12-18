#ifndef FLIGHT_UPDATE_DTO_H
#define FLIGHT_UPDATE_DTO_H

#include <QHttpServerRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class FlightUpdateDTO {
public:
    QString authCode;
    int flightId;
    QString flightNumber;
    QString departureCity;
    QString arrivalCity;
    QDateTime departureTime;
    QDateTime arrivalTime;
    double price;
    QString departureAirport;
    QString arrivalAirport;
    QString airlineCompany;
    QDateTime checkinStartTime;
    QDateTime checkinEndTime;
    QString status;

    FlightUpdateDTO(const QHttpServerRequest &request);

    bool isValid() const;
};

#endif // FLIGHT_UPDATE_DTO_H
