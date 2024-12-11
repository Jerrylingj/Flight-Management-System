#include "OrderApi.h"
#include "dto/response_dto.h"
#include "dto/order_info_dto.h"
#include <QJsonArray>

QJsonObject getOrder(DatabaseManager* m_db){
    try{
        QJsonArray orders;
        m_db->queryOrder(orders);
        auto response = success(orders);
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("在getOrder函数中获取数据失败");
        return response->toJson();
    }
}

// QJsonObject getOrder(int orderID, DatabaseManager* m_db){
//     try{
//         OrderInfo order;
//         m_db->queryOrder(orderID, order);
//         auto response = success(order.toJson());
//         return response->toJson();
//     }catch(const std::invalid_argument& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::runtime_error& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::exception& e){
//         auto response = fail<QJsonObject>("失败");
//         return response->toJson();
//     }
// }

// QJsonObject createOrder(const OrderInfo& orderInfo, DatabaseManager* m_db){
//     try{
//         m_db->insertOrder(orderInfo);
//         auto response = success<QString>("订单创建成功");
//         return response->toJson();
//     }catch(const std::runtime_error& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::exception& e){
//         auto response = fail<QJsonObject>("失败");
//         return response->toJson();
//     }
// }

// QJsonObject updateOrder(int orderID, const OrderInfo& orderInfo, DatabaseManager* m_db){
//     try{
//         m_db->updateOrder(orderID, orderInfo);
//         auto response = success<QString>("订单更新成功");
//         return response->toJson();
//     }catch(const std::invalid_argument& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::runtime_error& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::exception& e){
//         auto response = fail<QJsonObject>("失败");
//         return response->toJson();
//     }
// }

// QJsonObject deleteOrder(int orderID, DatabaseManager* m_db){
//     try{
//         m_db->deleteOrder(orderID);
//         auto response = success<QString>("订单删除成功");
//         return response->toJson();
//     }catch(const std::invalid_argument& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::runtime_error& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::exception& e){
//         auto response = fail<QJsonObject>("失败");
//         return response->toJson();
//     }
// }
