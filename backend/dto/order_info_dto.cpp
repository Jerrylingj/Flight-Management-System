#include "order_info_dto.h"

OrderInfo::OrderInfo()
    : orderId(0), userId(0), flightId(0), amount(0.0)
{
    // 默认构造函数
    qDebug() << "创建了一个默认的 OrderInfo 对象。";
}

QJsonObject OrderInfo::toJson() const
{
    QJsonObject json;
    json["orderId"] = orderId;
    json["userId"] = userId;
    json["flightId"] = flightId;
    json["flightNumber"] = flightNumber;
    json["departure"] = departure;
    json["destination"] = destination;
    json["departureTime"] = departureTime.toString(Qt::ISODate);
    json["arrivalTime"] = arrivalTime.toString(Qt::ISODate);
    json["departureAirport"] = departureAirport;
    json["arrivalAirport"] = arrivalAirport;
    json["checkInStartTime"] = checkInStartTime.toString(Qt::ISODate);
    json["checkInEndTime"] = checkInEndTime.toString(Qt::ISODate);
    json["airlineCompany"] = airlineCompany;
    json["status"] = status;
    json["orderDate"] = orderDate;
    json["amount"] = amount;

    // 调试输出转换后的 JSON 数据
    qDebug() << "OrderInfo 转换为 JSON 数据：" << json;
    return json;
}

void OrderInfo::fromJson(const QJsonObject& json)
{
    if (json.contains("orderId")) {
        orderId = json["orderId"].toInt();
    }
    if (json.contains("userId")) {
        userId = json["userId"].toInt();
    }
    if (json.contains("flightId")) {
        flightId = json["flightId"].toInt();
    }
    if (json.contains("flightNumber")) {
        flightNumber = json["flightNumber"].toString();
    }
    if (json.contains("departure")) {
        departure = json["departure"].toString();
    }
    if (json.contains("destination")) {
        destination = json["destination"].toString();
    }
    if (json.contains("departureTime")) {
        departureTime = QDateTime::fromString(json["departureTime"].toString(), Qt::ISODate);
    }
    if (json.contains("arrivalTime")) {
        arrivalTime = QDateTime::fromString(json["arrivalTime"].toString(), Qt::ISODate);
    }
    if (json.contains("departureAirport")) {
        departureAirport = json["departureAirport"].toString();
    }
    if (json.contains("arrivalAirport")) {
        arrivalAirport = json["arrivalAirport"].toString();
    }
    if (json.contains("checkInStartTime")) {
        checkInStartTime = QDateTime::fromString(json["checkInStartTime"].toString(), Qt::ISODate);
    }
    if (json.contains("checkInEndTime")) {
        checkInEndTime = QDateTime::fromString(json["checkInEndTime"].toString(), Qt::ISODate);
    }
    if (json.contains("airlineCompany")) {
        airlineCompany = json["airlineCompany"].toString();
    }
    if (json.contains("status")) {
        status = json["status"].toString();
    }
    if (json.contains("orderDate")) {
        orderDate = json["orderDate"].toString();
    }
    if (json.contains("amount")) {
        amount = json["amount"].toDouble();
    }

    // 调试输出解析后的数据
    qDebug() << "从 JSON 数据解析 OrderInfo 对象，字段信息如下：";
    qDebug() << "订单 ID:" << orderId;
    qDebug() << "用户 ID:" << userId;
    qDebug() << "航班 ID:" << flightId;
    qDebug() << "航班号:" << flightNumber;
    qDebug() << "出发地:" << departure;
    qDebug() << "目的地:" << destination;
    qDebug() << "出发时间:" << departureTime.toString(Qt::ISODate);
    qDebug() << "到达时间:" << arrivalTime.toString(Qt::ISODate);
    qDebug() << "出发机场:" << departureAirport;
    qDebug() << "到达机场:" << arrivalAirport;
    qDebug() << "检票开始时间:" << checkInStartTime.toString(Qt::ISODate);
    qDebug() << "检票结束时间:" << checkInEndTime.toString(Qt::ISODate);
    qDebug() << "航空公司:" << airlineCompany;
    qDebug() << "状态:" << status;
    qDebug() << "订单日期:" << orderDate;
    qDebug() << "金额:" << amount;
}
