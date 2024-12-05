#ifndef LOGIN_H
#define LOGIN_H

#include "database/databasemanager.h"
#include <QHttpServer>
#include <QJsonObject>
#include <QJsonDocument>

QJsonObject login(const QHttpServerRequest &request, DatabaseManager* m_db);

#endif // LOGIN_H
