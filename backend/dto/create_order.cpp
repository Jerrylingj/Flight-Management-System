#include "create_order.h"

CreateOrderRequest::CreateOrderRequest(QJsonObject& obj){
    if(obj.contains("flightID")){
        flightID = obj.value("flightID").toInt();
    }else {
        throw std::invalid_argument("缺少参数flightID");
    }
}
