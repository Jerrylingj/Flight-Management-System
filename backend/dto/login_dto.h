#ifndef LOGIN_DTO_H
#define LOGIN_DTO_H

#include <QObject>
#include <QString>
#include <QJsonObject>

class LoginDTO : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString telephone READ telephone CONSTANT)
    Q_PROPERTY(QString password READ password CONSTANT)
public:
    explicit LoginDTO(const QJsonObject& jsonObj, QObject* parent = nullptr);
    QString telephone() const {
        return m_telephone;
    }
    QString password() const {
        return m_password;
    }
private:
    QString m_telephone;
    QString m_password;
};

class LoginReturnDTO : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString telephone READ telephone CONSTANT)
    Q_PROPERTY(QString token READ token CONSTANT)
public:
    explicit LoginReturnDTO(QObject* parent = nullptr) : QObject(parent) {}
    explicit LoginReturnDTO(const QJsonObject& jsonObj, QObject* parent = nullptr);
    QString telephone() const {
        return m_telephone;
    }
    QString token() const {
        return m_token;
    }
    QJsonObject toJson() const;
private:
    QString m_telephone;
    QString m_token;
};

#endif // LOGIN_DTO_H
