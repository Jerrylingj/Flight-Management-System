#include "login.h"

#include "dto/response_dto.h"
#include "dto/login_dto.h"
#include <QJsonDocument>

QJsonObject login(const QHttpServerRequest &request, DatabaseManager* m_db){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        try {
            LoginDTO info(jsonObj);
            int id = m_db->queryUsers(info.getTelephone(),info.getPassword());
            if(id != -1){
                QJsonObject userInfo;
                userInfo["userID"] = id;
                userInfo["telephone"] = info.getTelephone();
                LoginReturnDTO loginReturn(userInfo);
                auto response = success<QJsonObject>(loginReturn.toJson());
                return response->toJson();
            }else{
                auto response = fail<QJsonObject>("账号或密码错误");
                return response->toJson();
            }
        } catch (const std::invalid_argument& e) {
            auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
            return response->toJson();
        } catch (const std::exception& e) {
            auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
            return response->toJson();
        }
    }
    auto response = fail<QJsonObject>("failed");
    return response->toJson();
}
