#include "flight_del_dto.h"
#include <stdexcept>

FlightDelDTO::FlightDelDTO(const QHttpServerRequest &request) {
    // 将请求体解析为 JSON 对象
    QJsonDocument body = QJsonDocument::fromJson(request.body());
    QJsonObject json = body.object();

    authCode = json.value("authCode").toString();
    flightId = json.value("flightId").toInt(-1); // 默认值为 -1，表示无效

    // 检查必需字段
    if (authCode.isEmpty()) {
        throw std::invalid_argument("authCode is missing");
    }
    if (flightId == -1) {
        throw std::invalid_argument("flightId is missing or invalid");
    }
}

QString FlightDelDTO::getAuthCode() const {
    return authCode;
}

int FlightDelDTO::getFlightId() const {
    return flightId;
}

