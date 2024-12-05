#include "login_dto.h"
#include <stdexcept>

LoginDTO::LoginDTO(const QJsonObject& jsonObj, QObject* parent):QObject(parent){
    if (!jsonObj.contains("telephone") || !jsonObj["telephone"].isString()) {
        throw std::invalid_argument("Invalid or missing telephone number");
    }
    m_telephone = jsonObj["telephone"].toString();

    if (!jsonObj.contains("password") || !jsonObj["password"].isString()) {
        throw std::invalid_argument("Invalid or missing password");
    }
    m_password = jsonObj["password"].toString();
}

LoginReturnDTO::LoginReturnDTO(const QJsonObject& jsonObj, QObject* parent):QObject(parent){
    // 之后再写加密函数
    m_telephone = jsonObj.value("telephone").toString();
    m_token = jsonObj.value("telephone").toString();
}

QJsonObject LoginReturnDTO::toJson() const {
    QJsonObject obj;
    obj["telephone"] = m_telephone;
    obj["token"] = m_token;
    return obj;
}
