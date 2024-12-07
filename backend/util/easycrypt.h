#ifndef EASYCRYPT_H
#define EASYCRYPT_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>

QString encrypt(const QJsonObject& jsonObject);

QJsonObject decrypt(const QString& encryptedString);

#endif // EASYCRYPT_H
