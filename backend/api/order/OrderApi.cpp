#include "OrderApi.h"
#include "dto/response_dto.h"
#include "dto/order_info_dto.h"
#include <QJsonArray>

// // 已经有了处理token的函数，所以该函数不应当被调用
// // 如果后续增加管理员端，可以启用下列函数
// QJsonObject getOrder(DatabaseManager* m_db){
//     try{
//         QJsonArray orders;
//         m_db->queryOrder(orders);
//         auto response = success(orders);
//         return response->toJson();
//     }catch(const std::runtime_error& e){
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }catch(const std::exception& e){
//         auto response = fail<QJsonObject>("在getOrder函数中获取数据失败");
//         return response->toJson();
//     }
// }


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

// 获取当前用户的所有航班信息
QJsonObject getOrder(DatabaseManager* m_db, int userId){
    if (!m_db) {
        auto response = fail<QJsonObject>("[错误] getORder 函数异常: DatabaseManager 指针为空");
        return response->toJson();
    }

    try{
        QJsonArray orders;
        m_db->queryOrder(orders, userId);
        return success(orders)->toJson();
    }
    catch(const std::exception& e){
        qDebug() << "[错误] getOrder 函数异常: " << e.what();
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }
}

// 预定功能
QJsonObject createOrder(const OrderInfo& orderInfo, DatabaseManager* m_db){
    try{
        m_db->insertOrder(orderInfo);
        auto response = success<QString>("订单创建成功");
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("失败");
        return response->toJson();
    }
}

// 支付功能
QJsonObject payOrder(int orderId, DatabaseManager* m_db){
    // ...
}

// 退签功能
QJsonObject deleteOrder(int orderID, DatabaseManager* m_db){
    try{
        m_db->deleteOrder(orderID);
        auto response = success<QString>("订单删除成功");
        return response->toJson();
    }catch(const std::invalid_argument& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("失败");
        return response->toJson();
    }
}

// 改签功能
QJsonObject updateOrder(int orderID, const OrderInfo& orderInfo, DatabaseManager* m_db){
    try{
        m_db->updateOrder(orderID, orderInfo);
        auto response = success<QString>("订单更新成功");
        return response->toJson();
    }catch(const std::invalid_argument& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("失败");
        return response->toJson();
    }
}
