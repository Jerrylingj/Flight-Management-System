#include "user.h"
#include "util/easycrypt.h"
#include "dto/response_dto.h"
#include "dto/user_dto.h"

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
    try{
        UpdateUserRequest req(request);
        int userId = getUserID(request);
        if(req.getBalance()){
            m_db->putUser(userId, req.getBalance());
            auto resp = success(QString("交易成功"));
            return resp->toJson();
        }
        if(req.getUsername().length()>0){
            m_db->putUser(userId,0,req.getUsername());
            auto resp = success(QString("修改用户名成功"));
            return resp->toJson();
        }
        if(req.getAvatarUrl().length()>0){
            m_db->putUser(userId, 1, req.getAvatarUrl());
            auto resp = success(QString("修改头像成功"));
            return resp->toJson();
        }
        throw std::runtime_error("未知错误");
    }catch(const std::invalid_argument& e){
        auto resp = fail<QJsonObject>(e.what());
        return resp->toJson();
    }catch(const std::exception& e){
        auto resp = fail<QJsonObject>(e.what());
        return resp->toJson();
    }
}

QJsonObject delUser(const QHttpServerRequest &request, DatabaseManager* m_db){
    try{
        int userId = getUserID(request);
        m_db->deleteUser(userId);
        auto resp = success(QString("删除成功"));
        return resp->toJson();
    }catch(const std::exception& e){
        auto resp = fail<QJsonObject>(e.what());
        return resp->toJson();
    }
}

QJsonObject isAuth(const QHttpServerRequest &request){
    try{
        QByteArray body = request.body();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
        if(jsonDoc.isNull()||!jsonDoc.isObject()){
            throw std::invalid_argument("无效的body");
        }
        QJsonObject obj = jsonDoc.object();
        if(!obj.contains("authCode")){
            throw std::invalid_argument("没有authCode");
        }
        if(obj.value("authCode").toString() != "123"){
            throw std::invalid_argument("不是管理员");
        }
        auto resp = success(true);
        return resp->toJson();
    }catch(const std::exception& e){
        auto resp = fail<QJsonObject>(e.what());
        return resp->toJson();
    }
}
