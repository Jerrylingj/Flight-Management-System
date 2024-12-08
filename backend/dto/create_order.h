#ifndef DTO_CREATE_ORDER_H
#define DTO_CREATE_ORDER_H


#include <QJsonObject>

class CreateOrderRequest
{
public:
    CreateOrderRequest(QJsonObject& obj);
    int getFlightID() const {
        return flightID;
    }
private:
    int flightID;
};

#endif // CREATE_ORDER_H
