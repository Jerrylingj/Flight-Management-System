#ifndef FLIGHTAPI_H
#define FLIGHTAPI_H

#include "database/databasemanager.h"
#include <QHttpServer>

// 获取所有航班
QJsonObject getFlight(DatabaseManager *m_db);
// 获取特定航班航班
QJsonObject getFlight(int flightID, DatabaseManager *m_db);
// 动态获取航班
QJsonObject getFlight(const QHttpServerRequest &request, DatabaseManager *m_db);
// 用于改签：获取和flightId所指向的航班相同出发地和目的地的、出发时间晚于flightId所指向的航班，在当天、明天或后天触发的航班，转化为QJsonObject，返还给Controller层
QJsonObject getNextFlight(int flightId, DatabaseManager *m_db);
// 删除航班
QJsonObject deleteFlight(const QHttpServerRequest &request, DatabaseManager *m_db);
// 添加航班
QJsonObject addFlight(const QHttpServerRequest &request, DatabaseManager *m_db);
// 更新航班信息
QJsonObject updateFlight(const QHttpServerRequest &request, DatabaseManager *m_db);

#endif // FLIGHTAPI_H
