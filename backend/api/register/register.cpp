#include "register.h"
#include "dto/response_dto.h"
#include "dto/send_code.h"
#include "dto/register_dto.h"
#include "util/networkhandler.h"

QJsonObject registerUser(const QHttpServerRequest &request, DatabaseManager* m_db){
    try{
        RegisterRequest req(request);
        if(m_db->queryUsers(req.getEmail())){
            auto response = fail<QJsonObject>(QString("该账号已注册过"));
            return response->toJson();
        }else {
            if(m_db->insertUser(req.getUsername(), req.getEmail(), req.getPassword())){
                auto response = success(QString("注册成功!"));
                return response->toJson();
            }else{
                auto reponse = fail<QJsonObject>("服务器错误");
                return reponse->toJson();
            }
        }
    }catch(const std::invalid_argument& e){
        auto response = fail<QJsonObject>(e.what());
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>(e.what());
        return response->toJson();
    }
}

QJsonObject sendCode(const QHttpServerRequest& request){
    try{
        SendCodeRequest req(request);
        QJsonObject requestBody;
        requestBody["email"] = req.getEmail();
        requestBody["code"] = "123456"; // 需要一个生成随机数的函数
        QJsonObject resp = useNetwork("http://localhost:3000/api/send-code", "POST", requestBody);
        if((!resp.contains("error"))&&(resp.value("error").isNull()||resp.value("error").isUndefined())){
            SendCodeResponse returnValue(requestBody);
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
