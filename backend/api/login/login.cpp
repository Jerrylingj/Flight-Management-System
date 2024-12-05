#include "login.h"

#include "dto/response_dto.h"

QJsonObject login(const QHttpServerRequest &request, DatabaseManager* m_db){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();

        QString telephone;
        QString password;
        if(jsonObj.contains("telephone")&&jsonObj["telephone"].isString()){
            telephone = jsonObj["telephone"].toString();
        }else {
            auto response = fail<QJsonObject>("请输入手机号");
            return response->toJson();
        }
        if(jsonObj.contains("password")&&jsonObj["password"].isString()){
            password = jsonObj["password"].toString();
        }else {
            auto response = fail<QJsonObject>("请输入密码");
            return response->toJson();
        }
        if(m_db->queryUsers(telephone, password)){
            // 发放token
            auto response = success(QString("登陆成功"));
            return response->toJson();
        }else {
            auto response = fail<QJsonObject>("账号或密码错误");
            return response->toJson();
        }
    }
    auto response = fail<QJsonObject>("failed");
    return response->toJson();
}
