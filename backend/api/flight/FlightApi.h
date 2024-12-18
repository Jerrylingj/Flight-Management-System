#ifndef FLIGHTAPI_H
#define FLIGHTAPI_H

#include "database/databasemanager.h"
#include <QHttpServer>

QJsonObject getFlight(DatabaseManager* m_db);

QJsonObject getFlight(int flightID,DatabaseManager* m_db);

QJsonObject getFlight(const QHttpServerRequest &request, DatabaseManager* m_db);

// 删除航班
QJsonObject deleteFlight(const QHttpServerRequest &request, DatabaseManager* m_db);

// 更新航班信息
QJsonObject updateFlight(const QHttpServerRequest &request, DatabaseManager* m_db);

#endif // FLIGHTAPI_H
