#ifndef SEND_CODE_H
#define SEND_CODE_H

#include <QHttpServer>
#include <QJsonObject>

class SendCodeRequest
{
public:
    SendCodeRequest(const QHttpServerRequest &request);
    QString getEmail() const{
        return email;
    }
private:
    QString email;
};

class SendCodeResponse
{
public:
    SendCodeResponse(const QJsonObject& obj);
    QString getHashValue() const {
        return hashValue;
    }
private:
    QString hashValue;
};

#endif // SEND_CODE_H
