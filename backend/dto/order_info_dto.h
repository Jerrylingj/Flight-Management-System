#ifndef ORDER_INFO_DTO_H
#define ORDER_INFO_DTO_H

#include <QJsonObject>

class OrderInfo {
public:
    int orderID;
    int userID;
    int flightID;
    QString orderDate;
    double amount;

    OrderInfo();

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
};

#endif // ORDER_INFO_DTO_H