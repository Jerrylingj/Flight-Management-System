#include "user_dto.h"
#include "util/easycrypt.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


/*** 用户注册 ***/
RegisterRequest::RegisterRequest(const QHttpServerRequest &request){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        QString value;
        QString code;
        if(jsonObj.contains("value")&&jsonObj["value"].isString()){
            value = jsonObj["value"].toString();
        }else {
            throw std::invalid_argument("请先发送验证码");
        }
        if(jsonObj.contains("email")&&jsonObj["email"].isString()){
            email = jsonObj["email"].toString();
        }else {
            throw std::invalid_argument("缺少邮箱");
        }
        if(jsonObj.contains("code")&&jsonObj["code"].isString()){
            code = jsonObj["code"].toString();
        }else {
            throw std::invalid_argument("缺少验证码");
        }
        QJsonObject obj;
        obj["email"] = email;
        obj["code"] = code;
        QString hashValue = hashText(obj);
        if (value != hashValue) {
            throw std::invalid_argument("验证码错误");
        }
        if(jsonObj.contains("username")&&jsonObj["username"].isString()){
            username = jsonObj["username"].toString();
        }else {
            throw std::invalid_argument("缺少用户名");
        }
        if(jsonObj.contains("password")&&jsonObj["password"].isString()){
            password = jsonObj["password"].toString();
            // 判断邮箱是否正确
            static const QRegularExpression passwordRegex(R"(^\S*(?=\S{6,12})(?=\S*\d)(?=\S*[A-Z])(?=\S*[a-z])(?=\S*[!@#$%^&*? ])\S*$)");
            QRegularExpressionMatch match = passwordRegex.match(password);

            // 如果邮箱格式不匹配，抛出异常
            if (!match.hasMatch()) {
                throw std::invalid_argument("需要6-12字符，需要大写、小写字母和特殊符号");
            }
        }else {
            throw std::invalid_argument("缺少密码");
        }
    }
}


/*** 用户登录 ***/
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


/*** 用户信息更新 ***/
UpdateUserRequest::UpdateUserRequest(const QHttpServerRequest &request){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj.contains("balance") && jsonObj["balance"].isDouble()) {
            balance = jsonObj.value("balance").toDouble();
            return;
        }
        if (jsonObj.contains("avatar_url") && jsonObj["avatar_url"].isString()) {
            avatar_url = jsonObj.value("avatar_url").toString();
            return;
        }
        if (jsonObj.contains("username") && jsonObj["username"].isString()) {
            username = jsonObj.value("username").toString();
            return;
        }
        throw std::invalid_argument("缺少需要的参数");
    }else{
        throw std::invalid_argument("错误的请求体");
    }
}

