#include "order_dto.h"

/*** 订单信息 ***/
OrderInfo::OrderInfo()
    : orderId(0), userId(0), flightId(0)
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
    json["totalChangeCount"] = totalChangeCount;
    json["paymentStatus"] = paymentStatus; // 注意此处，QJsonObject会自动处理bool值

    json["flightNumber"] = flightNumber;
    json["airlineCompany"] = airlineCompany;
    json["price"] = price;
    json["flightStatus"] = flightStatus;

    json["departure"] = departure;
    json["destination"] = destination;
    json["departureTime"] = departureTime.toString(Qt::ISODate);
    json["arrivalTime"] = arrivalTime.toString(Qt::ISODate);
    json["departureAirport"] = departureAirport;
    json["arrivalAirport"] = arrivalAirport;

    json["checkInStartTime"] = checkInStartTime.toString(Qt::ISODate);
    json["checkInEndTime"] = checkInEndTime.toString(Qt::ISODate);

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
    if (json.contains("totalChangeCount")) {
        totalChangeCount = json["totalChangeCount"].toInt();
    }
    if (json.contains("paymentStatus")) {
        paymentStatus = json["paymentStatus"].toBool(); // 注意此处，QJsonObject会正确处理bool值
    }
    if (json.contains("flightNumber")) {
        flightNumber = json["flightNumber"].toString();
    }
    if (json.contains("airlineCompany")) {
        airlineCompany = json["airlineCompany"].toString();
    }
    if (json.contains("price")) {
        price = json["price"].toDouble();
    }
    if (json.contains("flightStatus")) {
        flightStatus = json["flightStatus"].toString();
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

    // 调试输出解析后的数据
    qDebug() << "从 JSON 数据解析 OrderInfo 对象，字段信息如下：";
    qDebug() << "订单 ID:" << orderId;
    qDebug() << "用户 ID:" << userId;
    qDebug() << "航班 ID:" << flightId;
    qDebug() << "总改签次数:" << totalChangeCount;
    qDebug() << "支付状态:" << paymentStatus;
    qDebug() << "航班号:" << flightNumber;
    qDebug() << "航空公司:" << airlineCompany;
    qDebug() << "票价:" << price;
    qDebug() << "航班状态:" << flightStatus;
    qDebug() << "出发地:" << departure;
    qDebug() << "目的地:" << destination;
    qDebug() << "出发时间:" << departureTime.toString(Qt::ISODate);
    qDebug() << "到达时间:" << arrivalTime.toString(Qt::ISODate);
    qDebug() << "出发机场:" << departureAirport;
    qDebug() << "到达机场:" << arrivalAirport;
    qDebug() << "检票开始时间:" << checkInStartTime.toString(Qt::ISODate);
    qDebug() << "检票结束时间:" << checkInEndTime.toString(Qt::ISODate);
}

/*** 订单创建 ***/
CreateOrderRequest::CreateOrderRequest(QJsonObject& obj){
    if(obj.contains("flightID")){
        flightID = obj.value("flightID").toInt();
    }else {
        throw std::invalid_argument("缺少参数flightID");
    }
}

