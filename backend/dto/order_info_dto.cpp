#include "order_info_dto.h"

OrderInfo::OrderInfo()
    : orderID(0), userID(0), flightID(0), amount(0.0)
{
}

QJsonObject OrderInfo::toJson() const {
    QJsonObject json;
    json["orderID"] = orderID;
    json["userID"] = userID;
    json["flightID"] = flightID;
    json["orderDate"] = orderDate;
    json["amount"] = amount;
    return json;
}

void OrderInfo::fromJson(const QJsonObject& json) {
    if (json.contains("orderID"))
        orderID = json["orderID"].toInt();
    if (json.contains("userID"))
        userID = json["userID"].toInt();
    if (json.contains("flightID"))
        flightID = json["flightID"].toInt();
    if (json.contains("orderDate"))
        orderDate = json["orderDate"].toString();
    if (json.contains("amount"))
        amount = json["amount"].toDouble();
}