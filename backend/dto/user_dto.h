#ifndef USER_DTO_H
#define USER_DTO_H

#include <QString>
#include <QJsonObject>
#include <QHttpServer>

//用户注册
class RegisterRequest
{
public:
    RegisterRequest(const QHttpServerRequest &request);
    QString getUsername() const {
        return username;
    }
    QString getEmail() const {
        return email;
    }
    QString getPassword() const {
        return password;
    }
private:
    QString username;
    QString email;
    QString password;
};


// 用户登录
class LoginDTO
{
public:
    LoginDTO(const QHttpServerRequest &request);
    QString getEmail() const {
        return m_email;
    }
    QString getPassword() const {
        return m_password;
    }
private:
    QString m_email;
    QString m_password;
};

class LoginReturnDTO
{
public:
    LoginReturnDTO(const QJsonObject& jsonObj);
    QString getToken() const {
        return m_token;
    }
    QJsonObject toJson() const;
private:
    QString m_token;
};

// 用户信息更新
class UpdateUserRequest
{
public:
    UpdateUserRequest(const QHttpServerRequest &request);
    QString getAvatarUrl() const{
        return avatar_url;
    }
    QString getUsername() const{
        return username;
    }
    double getBalance() const{
        return balance;
    }
private:
    QString avatar_url = "";
    QString username = "";
    double balance = 0.0;
};

#endif // USER_DTO_H
