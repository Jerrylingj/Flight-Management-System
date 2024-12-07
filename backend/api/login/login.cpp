#include "login.h"

#include "dto/response_dto.h"
#include "dto/login_dto.h"

QJsonObject login(const QHttpServerRequest &request, DatabaseManager* m_db){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        try {
            LoginDTO info(jsonObj);
            if(m_db->queryUsers(info.getTelephone(),info.getPassword())){
                LoginReturnDTO loginReturn(jsonObj);
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
