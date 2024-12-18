#ifndef FLIGHT_DEL_DTO_H
#define FLIGHT_DEL_DTO_H

#include <QHttpServer>
#include <QJsonDocument>
#include <QJsonObject>

class FlightDelDTO
{
public:
    // 从请求中解析数据
    FlightDelDTO(const QHttpServerRequest &request);

    QString getAuthCode() const;

    int getFlightId() const;

private:
    QString authCode;
    int flightId;
};

#endif // FLIGHT_DEL_DTO_H
