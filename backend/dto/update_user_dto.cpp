#include "update_user_dto.h"

#include <QJsonDocument>

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
