#include"userlist_dto.h"
#include<QHttpServerRequest>
#include <QJsonDocument>

USERLIST::USERLIST(const QHttpServerRequest &request){
    QJsonDocument body = QJsonDocument::fromJson(request.body());
    QJsonObject json = body.object();
    verify_code = json.value("authCode").toString();
}
QString USERLIST::GetVerifyCode(){
    return verify_code;
}

USERINFORMATION::USERINFORMATION(QString name, QString avator, int money, QString email, QString password){
    user_name = name;
    user_avator = avator;
    user_money = money;
    user_email = email;
    user_password = password;
}
