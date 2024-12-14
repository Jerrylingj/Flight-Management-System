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

// 加密文本，无法解密(很难解密)
QString hashText(const QString& text);
QString hashText(const QJsonObject& jsonData);

#endif // EASYCRYPT_H
