#include"userlist.h"
#include"dto/userlist_dto.h"
#include <QHttpServer>
#include <QJsonObject>
#include<QJsonArray>
#include "database/databasemanager.h"
#include "dto/response_dto.h"
QJsonObject getAllUserInfo(const QHttpServerRequest &request, DatabaseManager* m_db){
    USERLIST user(request);
    if(user.GetVerifyCode() != "123"){
        auto response = fail<QJsonObject>("wrong code");
        return response->toJson();
    }
    QJsonArray userinfolist;
    try{
        m_db->queryAllUserList(userinfolist);
        auto responce = success(userinfolist);
        return responce->toJson();
    }
    catch(const std::exception& e){
        auto response = fail<QJsonObject>(e.what());
        return response->toJson();
    }
}
