#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QJsonObject>
#include<QNetworkAccessManager>

QJsonObject useNetwork(const QString& targetUrl, const QString& method, const QJsonObject& body, const QString& token = "");

#endif // NETWORKHANDLER_H
