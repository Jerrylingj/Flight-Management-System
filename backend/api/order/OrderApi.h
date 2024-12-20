#ifndef ORDERAPI_H
#define ORDERAPI_H

#include "database/databasemanager.h"
#include "dto/order_dto.h"

// // 如果后续增加管理员端，可以启用下列函数
// QJsonObject getOrder(DatabaseManager* m_db);、
// QJsonObject getOrder(int orderID, DatabaseManager* m_db);

// 获取当前用户的所有航班信息
QJsonObject getOrder(DatabaseManager* m_db, int userId);

// 预定功能
QJsonObject createOrder(int userId, int flightId, DatabaseManager* m_db);

// 支付功能
QJsonObject payOrder(int orderId, DatabaseManager* m_db);

// 退签功能
QJsonObject unpayOrder(int orderId, DatabaseManager* m_db);

// 删除（取消支付）功能
QJsonObject deleteOrder(int orderId, DatabaseManager* m_db);

// 改签功能
QJsonObject rebookOrder(int orderId, int flightId, DatabaseManager* m_db);

#endif // ORDERAPI_H
