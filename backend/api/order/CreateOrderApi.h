#ifndef CREATE_ORDER_H
#define CREATE_ORDER_H

#include "database/databasemanager.h"
#include <QHttpServer>
#include <QJsonObject>

QJsonObject CreateOrder(const QHttpServerRequest &request, DatabaseManager* m_db);

#endif // CREATE_ORDER_H
