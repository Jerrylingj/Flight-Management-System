#ifndef UPDATE_USER_DTO_H
#define UPDATE_USER_DTO_H

#include <QHttpServer>
#include <QJsonObject>

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

#endif // UPDATE_USER_DTO_H
