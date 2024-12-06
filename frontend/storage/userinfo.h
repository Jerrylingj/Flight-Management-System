#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include <QString>

class UserInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString userPersonalInfo READ userPersonalInfo WRITE setUserPersonalInfo NOTIFY userPersonalInfoChanged)
    Q_PROPERTY(int myMoney READ myMoney WRITE setMyMoney NOTIFY myMoneyChanged)

public:
    explicit UserInfo(QObject *parent = nullptr) : QObject(parent) {}

    QString userName() const { return m_userName; }
    void setUserName(const QString &userName) {
        if (m_userName != userName) {
            m_userName = userName;
            emit userNameChanged();
        }
    }

    QString userPersonalInfo() const { return m_userPersonalInfo; }
    void setUserPersonalInfo(const QString &userPersonalInfo) {
        if (m_userPersonalInfo != userPersonalInfo) {
            m_userPersonalInfo = userPersonalInfo;
            emit userPersonalInfoChanged();
        }
    }

    int myMoney() const { return m_myMoney; }
    void setMyMoney(int myMoney) {
        if (m_myMoney != myMoney) {
            m_myMoney = myMoney;
            emit myMoneyChanged();
        }
    }

signals:
    void userNameChanged();
    void userPersonalInfoChanged();
    void myMoneyChanged();

private:
    QString m_userName;
    QString m_userPersonalInfo;
    int m_myMoney;
};

#endif // USERINFO_H
