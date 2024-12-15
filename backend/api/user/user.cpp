#include "user.h"
#include "util/easycrypt.h"
#include "dto/response_dto.h"

QJsonObject getUserInfo(const QHttpServerRequest &request, DatabaseManager* m_db){
    try{
        int userId = getUserID(request);
        QJsonObject obj;
        m_db->queryUser(userId, obj);
        auto resp = success(obj);
        return resp->toJson();
    }catch(const std::invalid_argument& e){
        auto resp = fail<QJsonObject>(e.what());
        return resp->toJson();
    }catch(const std::exception& e){
        auto resp = fail<QJsonObject>(e.what());
        return resp->toJson();
    }

}

QJsonObject updateUserInfo(const QHttpServerRequest &request, DatabaseManager* m_db){
    auto resp = success(QString("success"));
    return resp->toJson();
}
