#include "send_code.h"

#include <QJsonDocument>
#include <QRegularExpression>
#include "util/easycrypt.h"

SendCodeRequest::SendCodeRequest(const QHttpServerRequest &request) {
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("email") || !jsonObj["email"].isString()) {
            throw std::invalid_argument("Invalid or missing email");
        }
        email = jsonObj["email"].toString();

        // 判断邮箱是否正确
        static const QRegularExpression emailRegex(R"(^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$)");
        QRegularExpressionMatch match = emailRegex.match(email);

        // 如果邮箱格式不匹配，抛出异常
        if (!match.hasMatch()) {
            throw std::invalid_argument("Invalid email format");
        }
    }else{
        throw std::invalid_argument("错误的请求体");
    }
}

SendCodeResponse::SendCodeResponse(const QJsonObject& obj){
    hashValue = hashText(obj);
}
