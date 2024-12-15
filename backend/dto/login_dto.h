#ifndef LOGIN_DTO_H
#define LOGIN_DTO_H

#include <QString>
#include <QJsonObject>
#include <QHttpServer>

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
    QString getTelephone() const {
        return m_email;
    }
    QString getToken() const {
        return m_token;
    }
    QJsonObject toJson() const;
private:
    QString m_email;
    QString m_token;
};

#endif // LOGIN_DTO_H
