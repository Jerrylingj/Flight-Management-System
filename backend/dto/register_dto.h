#ifndef REGISTER_DTO_H
#define REGISTER_DTO_H

#include <QHttpServer>

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

#endif // REGISTER_DTO_H
