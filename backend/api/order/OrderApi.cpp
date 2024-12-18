#include "OrderApi.h"
#include "api/flight/FlightApi.h"
#include "dto/response_dto.h"
#include "dto/order_info_dto.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

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
        QJsonObject resp;
        resp["success"] = true;
        resp["data"] = orders;
        return resp;
    }
    catch(const std::exception& e){
        qDebug() << "[错误] getOrder 函数异常: " << e.what();
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }
}

// 创建订单函数
QJsonObject createOrder(int userId, int flightId, DatabaseManager* m_db) {
    QJsonObject response;

    if (!m_db) {
        response["success"] = false;
        response["message"] = "[错误] createOrder 函数异常: DatabaseManager 指针为空";
        return response;
    }

    try {
        // 调用 DatabaseManager 的 insertOrder 方法
        m_db->insertOrder(userId, flightId);

        // 构造成功的响应
        response["success"] = true;
        response["message"] = "Order created successfully";
        response["user_id"] = userId;
        response["flight_id"] = flightId;
    } catch (const std::exception &e) {
        // 捕获异常并返回错误信息
        response["success"] = false;
        response["message"] = QString("[错误] createOrder 函数异常: %1").arg(e.what());
    }

    return response;
}

// 支付功能
QJsonObject payOrder(int orderId, DatabaseManager* m_db) {
    QJsonObject response;

    if (!m_db) {
        response["success"] = false;
        response["code"] = 500;
        response["message"] = "支付失败：DatabaseManager 指针为空";
        return response;
    }

    try {
        // 仅更新 payment_status
        m_db->updatePaymentStatus(orderId, true);  // 设置支付状态为已支付

        response["success"] = true;
        response["code"] = 200;
        response["data"] = "订单支付成功";
        response["message"] = "操作成功";
        return response;
    }
    catch (const std::invalid_argument& e) {
        response["success"] = false;
        response["code"] = 400; // 根据需要设置合适的状态码
        response["message"] = QString::fromStdString(e.what());
        return response;
    }
    catch (const std::runtime_error& e) {
        response["success"] = false;
        response["code"] = 500; // 根据需要设置合适的状态码
        response["message"] = QString::fromStdString(e.what());
        return response;
    }
    catch (const std::exception& e) {
        response["success"] = false;
        response["code"] = 500; // 根据需要设置合适的状态码
        response["message"] = "支付失败";
        return response;
    }
}

// 改签功能
// QJsonObject rebookOrder(int orderID, int flightId, DatabaseManager* m_db){
//     if (!m_db) {
//         auto response = fail<QJsonObject>("[错误] reboogOrder 函数异常: DatabaseManager 指针为空");
//         return response->toJson();
//     }

//     try {
//         // 创建OrderInfo对象来更新订单信息
//         OrderInfo orderInfo;
//         orderInfo.orderId = orderID;
//         orderInfo.flightId = flightId;  // 更新为新的航班ID

//         // 需要增加改签次数
//         // 先查询当前订单信息
//         QJsonArray orders;
//         m_db->queryOrder(orders, orderInfo.userId);

//         // 遍历找到对应订单
//         for (const auto& order : orders) {
//             QJsonObject orderObj = order.toObject();
//             if (orderObj["orderId"].toInt() == orderID) {
//                 orderInfo.totalChangeCount = orderObj["totalChangeCount"].toInt() + 1;
//                 break;
//             }
//         }

//         // 更新订单信息
//         m_db->updateOrder(orderInfo);

//         auto response = success<QString>("改签成功");
//         return response->toJson();
//     }
//     catch (const std::invalid_argument& e) {
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }
//     catch (const std::runtime_error& e) {
//         auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//         return response->toJson();
//     }
//     catch (const std::exception& e) {
//         auto response = fail<QJsonObject>("改签失败");
//         return response->toJson();
//     }
// }

// 退签功能
QJsonObject deleteOrder(int orderId, DatabaseManager* m_db) {
    QJsonObject response;

    if (!m_db) {
        response["success"] = false;
        response["code"] = 500;
        response["message"] = "退签失败：DatabaseManager 指针为空";
        return response;
    }

    try {
        // 调用 DatabaseManager 的 deleteOrder 函数
        m_db->deleteOrder(orderId);

        response["success"] = true;
        response["code"] = 200;
        response["data"] = "订单退签成功";
        response["message"] = "操作成功";
        return response;
    }
    catch (const std::invalid_argument& e) {
        response["success"] = false;
        response["code"] = 400;
        response["message"] = QString::fromStdString(e.what());
        return response;
    }
    catch (const std::runtime_error& e) {
        response["success"] = false;
        response["code"] = 500;
        response["message"] = QString::fromStdString(e.what());
        return response;
    }
    catch (const std::exception& e) {
        response["success"] = false;
        response["code"] = 500;
        response["message"] = "退签失败";
        return response;
    }
}
