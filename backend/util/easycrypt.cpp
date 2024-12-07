#include "easycrypt.h"

// 加密函数
QString encrypt(const QJsonObject& jsonObject) {
    // 将QJsonObject转为字符串
    QJsonDocument jsonDoc(jsonObject);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    // 每个字符+2
    QByteArray byteArray = jsonString.toUtf8();
    for (char& c : byteArray) {
        c += 2;
    }

    // Base64加密
    QString encryptedString = byteArray.toBase64();
    return encryptedString;
}

// 解密函数
QJsonObject decrypt(const QString& encryptedString) {
    // Base64解密
    QByteArray byteArray = QByteArray::fromBase64(encryptedString.toUtf8());

    // 每个字符-2
    for (char& c : byteArray) {
        c -= 2;
    }

    // 将字符串转为QJsonObject
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoc.object();
    return jsonObject;
}
