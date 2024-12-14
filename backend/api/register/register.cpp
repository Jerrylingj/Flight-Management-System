#include "register.h"
#include "dto/response_dto.h"
#include "dto/send_code.h"
#include "util/networkhandler.h"

QJsonObject registerUser(const QHttpServerRequest &request, DatabaseManager* m_db){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();

        QString username;
        QString telephone;
        QString password;
        if(jsonObj.contains("username")&&jsonObj["username"].isString()){
            username = jsonObj["username"].toString();
        }else {
            auto response = fail<QJsonObject>("请输入用户名");
            return response->toJson();
        }
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
        if(m_db->queryUsers(telephone)){
            auto response = fail<QJsonObject>(QString("该账号已注册过"));
            return response->toJson();
        }else {
            if(m_db->insertUser(username, telephone, password)){
                auto response = success(QString("注册成功!"));
                return response->toJson();
            }else{
                auto reponse = fail<QJsonObject>("服务器错误");
                return reponse->toJson();
            }
        }
    }
    auto response = fail<QJsonObject>("failed");
    return response->toJson();
}

QJsonObject sendCode(const QHttpServerRequest& request){
    try{
        SendCodeRequest req(request);
        QJsonObject requestBody;
        requestBody["email"] = req.getEmail();
        requestBody["code"] = "123456"; // 需要一个生成随机数的函数
        QJsonObject resp = useNetwork("http://localhost:3000/api/send-code", "POST", requestBody);
        if((!resp.contains("error"))&&(resp.value("error").isNull()||resp.value("error").isUndefined())){
            SendCodeResponse returnValue(resp);
            auto response = success(returnValue.getHashValue());
            return response->toJson();
        }else{
            throw std::runtime_error("微服务出错");
        }
    }catch(const std::invalid_argument& e){
        auto response = fail<QJsonObject>(e.what());
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>(e.what());
        return response->toJson();
    }
}
