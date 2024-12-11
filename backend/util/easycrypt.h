#ifndef EASYCRYPT_H
#define EASYCRYPT_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QHttpServer>

///
/// 加密
/// {
///  userID:int,
///  telephone:string
/// }
///
///
QString encrypt(const QJsonObject& jsonObject);

///
/// 解密
/// {
///  userID:int,
///  telephone:string
/// }
///
///
QJsonObject decrypt(const QString& encryptedString);

int getUserID(const QHttpServerRequest &request);

#endif // EASYCRYPT_H
