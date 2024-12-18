#include "flight_dto.h"
#include <QJsonValue>
#include <stdexcept>

/*** 航班信息 ***/
FlightInfo::FlightInfo() : flightId(0), price(0.0) {
    // 默认构造函数
    qDebug() << "创建了一个默认的 FlightInfo 对象。";
}

FlightInfo::FlightInfo(int id, const QString &flightNum, const QString &depCity, const QString &arrCity, const QDateTime &depTime, const QDateTime &arrTime,
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
    qDebug() << "出发时间:" << departureTime.toString();
    qDebug() << "到达时间:" << arrivalTime.toString();
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

/*** 航班更新 ***/
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


/*** 航班删除 ***/
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


/*** 航班收藏 ***/
// 默认构造函数
FavoriteDTO::FavoriteDTO()
    : favoriteId(0), userId(0), flightId(0)
{
}
// 参数化构造函数
FavoriteDTO::FavoriteDTO(int favId, int usrId, int fltId)
    : favoriteId(favId), userId(usrId), flightId(fltId)
{
}
// 转换成 JSON
QJsonObject FavoriteDTO::toJson() const {
    QJsonObject json;
    json["favoriteId"] = favoriteId;
    json["userId"] = userId;
    json["flightId"] = flightId;
    return json;
}
