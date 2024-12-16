#ifndef ORDERAPI_H
#define ORDERAPI_H

#include "database/databasemanager.h"
#include "dto/order_info_dto.h"

// // 已经有了处理token的函数，所以该函数不应当被调用
// // 如果后续增加管理员端，可以启用该函数
// QJsonObject getOrder(DatabaseManager* m_db);

QJsonObject getOrder(DatabaseManager* m_db, int userId);

QJsonObject getOrder(int orderID, DatabaseManager* m_db);

QJsonObject createOrder(const OrderInfo& orderInfo, DatabaseManager* m_db);

QJsonObject deleteOrder(int orderID, DatabaseManager* m_db);

#endif // ORDERAPI_H
