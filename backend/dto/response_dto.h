// response_dto.h
#ifndef RESPONSE_DTO_H
#define RESPONSE_DTO_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMetaProperty>

// 非模板基类
class ResponseDTOBase : public QObject
{
    Q_OBJECT
public:
    explicit ResponseDTOBase(QObject *parent = nullptr);

    void setCode(int code) { m_code = code; }
    void setMessage(const QString &message) { m_message = message; }
    void setSuccess(bool success) { m_success = success; }

protected:
    int m_code;
    QString m_message;
    bool m_success;
};

// 模板派生类，不使用 Q_OBJECT
template<typename T>
class ResponseDTO : public ResponseDTOBase
{
public:
    explicit ResponseDTO(QObject *parent = nullptr) :
        ResponseDTOBase(parent) {}

    void setData(const T &data) { m_data = data; }

    QJsonObject toJson() const {
        QJsonObject json;
        json["code"] = m_code;
        json["message"] = m_message;
        json["success"] = m_success;

        if constexpr (std::is_base_of_v<QObject, T>) {
            const QMetaObject *meta = m_data.metaObject();
            QJsonObject dataObj;
            for (int i = 0; i < meta->propertyCount(); ++i) {
                QMetaProperty prop = meta->property(i);
                dataObj[prop.name()] = QJsonValue::fromVariant(prop.read(&m_data));
            }
            json["data"] = dataObj;
        } else {
            json["data"] = QJsonValue::fromVariant(QVariant::fromValue(m_data));
        }

        return json;
    }

private:
    T m_data;
};

// 辅助函数返回智能指针
template<typename T>
std::shared_ptr<ResponseDTO<T>> success(const T &data, const QString &message = "操作成功") {
    auto resp = std::make_shared<ResponseDTO<T>>();
    resp->setData(data);
    resp->setMessage(message);
    return resp;
}

template<typename T>
std::shared_ptr<ResponseDTO<T>> fail(const QString &message, int code = 500) {
    auto resp = std::make_shared<ResponseDTO<T>>();
    resp->setSuccess(false);
    resp->setCode(code);
    resp->setMessage(message);
    return resp;
}

#endif // RESPONSE_DTO_H
