#include "login.h"
#include "dto/response_dto.h"

QJsonObject login(const QHttpServerRequest &request){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        qDebug() << "json: " << jsonObj;
        auto response = success(jsonObj);
        return response->toJson();
    }
    auto response = fail<QJsonObject>("failed");
    return response->toJson();
}
