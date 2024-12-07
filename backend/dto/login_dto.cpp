#include "login_dto.h"
#include "util/easycrypt.h"
#include <stdexcept>
#include <QDebug>

LoginDTO::LoginDTO(const QJsonObject& jsonObj){
    if (!jsonObj.contains("telephone") || !jsonObj["telephone"].isString()) {
        throw std::invalid_argument("Invalid or missing telephone number");
    }
    m_telephone = jsonObj["telephone"].toString();

    if (!jsonObj.contains("password") || !jsonObj["password"].isString()) {
        throw std::invalid_argument("Invalid or missing password");
    }
    m_password = jsonObj["password"].toString();
}

LoginReturnDTO::LoginReturnDTO(const QJsonObject& jsonObj){
    m_telephone = jsonObj.value("telephone").toString();
    m_token = encrypt(jsonObj);
}

QJsonObject LoginReturnDTO::toJson() const {
    QJsonObject obj;
    obj["telephone"] = m_telephone;
    obj["token"] = m_token;
    return obj;
}
