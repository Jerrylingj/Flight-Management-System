#ifndef USERLIST_DTO_H
#define USERLIST_DTO_H
#include <QString>
#include <QJsonObject>
#include <QHttpServer>

class USERLIST{
public:
    USERLIST(const QHttpServerRequest &request);
    QString GetVerifyCode();
private:
    QString verify_code;
};

class USERINFORMATION{
public:
    USERINFORMATION(QString name, QString avator, int money, QString email, QString password);
    QString user_name;
    QString user_avator;
    int user_money;
    QString user_email;
    QString user_password;
};

#endif // USERLIST_DTO_H
