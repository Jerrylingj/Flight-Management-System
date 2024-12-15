#include "easycrypt.h"
#include <QDebug>

// 加密函数
QString encrypt(const QJsonObject& jsonObject) {
    // 创建一个可变的副本
    QJsonObject modifiableJsonObject = jsonObject;

    // 将QJsonObject转为字符串
    modifiableJsonObject["timeStamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument jsonDoc(modifiableJsonObject);
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

    if (jsonObject.contains("timeStamp")) {
        QDateTime timeStamp = QDateTime::fromString(jsonObject["timeStamp"].toString(), Qt::ISODate);
        QDateTime currentTime = QDateTime::currentDateTime();
        if (timeStamp.isValid() && timeStamp.secsTo(currentTime) > 24 * 3600) { // 24小时 = 24 * 3600秒
            throw std::runtime_error("Data has expired");
        }
    } else {
        throw std::runtime_error("Invalid data: Missing timeStamp");
    }

    return jsonObject;
}

int getUserID(const QHttpServerRequest &request){
    QList<QPair<QByteArray, QByteArray>> headers = request.headers();
    QString authHeader;
    for (const auto &header : headers) {
        if (header.first.toLower() == "authorization") {
            authHeader = QString::fromUtf8(header.second);
            break;
        }
    }
    if (authHeader.isEmpty()) {
        throw std::invalid_argument("没有token");
    }
    QString token;
    if (authHeader.startsWith("Bearer ")) {
        token = authHeader.mid(7);
    } else {
        throw std::invalid_argument("token错误");
    }
    QJsonObject userInfo = decrypt(token);
    if(!userInfo.contains("userID")){
        throw std::invalid_argument("token错误");
    }else{
        return userInfo.value("userID").toInt();
    }
}

QString hashText(const QString& text) {
    QString salt = "fixed_salt_value"; // 使用固定盐值
    QString passwordWithSalt = text + salt;
    QByteArray hash;
    for (int i = 0; i < 10000; ++i) {
        hash = QCryptographicHash::hash(
            passwordWithSalt.toUtf8(),
            QCryptographicHash::Sha256
            );
        passwordWithSalt = hash.toHex();
    }
    return salt + "$" + QString::fromUtf8(hash.toHex());
}

QString hashText(const QJsonObject& jsonData){
    QJsonDocument jsonDoc(jsonData);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
    return hashText(jsonString);
}
