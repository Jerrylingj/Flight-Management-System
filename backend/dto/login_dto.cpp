#include "login_dto.h"
#include "util/easycrypt.h"
#include <stdexcept>
#include <QDebug>
#include <QJsonDocument>

LoginDTO::LoginDTO(const QHttpServerRequest &request){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("email") || !jsonObj["email"].isString()) {
            throw std::invalid_argument("Invalid or missing email");
        }
        m_email = jsonObj["email"].toString();

        if (!jsonObj.contains("password") || !jsonObj["password"].isString()) {
            throw std::invalid_argument("Invalid or missing password");
        }
        m_password = jsonObj["password"].toString();
    }else{
        throw std::invalid_argument("错误的请求体");
    }
}

LoginReturnDTO::LoginReturnDTO(const QJsonObject& jsonObj){
    m_token = encrypt(jsonObj);
}

QJsonObject LoginReturnDTO::toJson() const {
    QJsonObject obj;
    obj["token"] = m_token;
    return obj;
}
