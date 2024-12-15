#ifndef USER_H
#define USER_H

#include <QHttpServer>
#include <QJsonObject>
#include "database/databasemanager.h"

QJsonObject getUserInfo(const QHttpServerRequest &request, DatabaseManager* m_db);

QJsonObject updateUserInfo(const QHttpServerRequest &request, DatabaseManager* m_db);

#endif // USER_H
