#ifndef USER_LIST_H
#define USER_LIST_H

#include <QHttpServer>
#include <QJsonObject>
#include "database/databasemanager.h"

QJsonObject getAllUserInfo(const QHttpServerRequest &request, DatabaseManager* m_db);

#endif // USER_H
