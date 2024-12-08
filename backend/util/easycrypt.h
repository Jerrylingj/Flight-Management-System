#ifndef EASYCRYPT_H
#define EASYCRYPT_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>

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

#endif // EASYCRYPT_H
