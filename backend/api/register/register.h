#ifndef REGISTER_H
#define REGISTER_H

#include "database/databasemanager.h"
#include <QHttpServer>
#include <QJsonObject>
#include <QJsonDocument>

QJsonObject registerUser(const QHttpServerRequest &request, DatabaseManager* m_db);

QJsonObject sendCode(const QHttpServerRequest& request);

#endif // REGISTER_H
