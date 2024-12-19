#ifndef FLIGHTAPI_H
#define FLIGHTAPI_H

#include "database/databasemanager.h"
#include <QHttpServer>

QJsonObject getFlight(DatabaseManager *m_db);

QJsonObject getFlight(int flightID, DatabaseManager *m_db);

QJsonObject getFlight(const QHttpServerRequest &request, DatabaseManager *m_db);

// 用于改签：获取和flightId所指向的航班相同出发地和目的地的、出发时间晚于flightId所指向的航班，同时时间距离最近的那一个航班的航班信息，转化为QJsonObject，返还给Controller层
QJsonObject getNextFlight(int flightId, DatabaseManager *m_db);
// 删除航班
QJsonObject deleteFlight(const QHttpServerRequest &request, DatabaseManager *m_db);
// 添加航班
QJsonObject addFlight(const QHttpServerRequest &request, DatabaseManager *m_db);
// 更新航班信息
QJsonObject updateFlight(const QHttpServerRequest &request, DatabaseManager *m_db);

#endif // FLIGHTAPI_H
