#include "CreateOrderApi.h"
#include <QJsonDocument>
#include "dto/response_dto.h"
#include "dto/create_order.h"
#include "util/easycrypt.h"

QJsonObject CreateOrder(const QHttpServerRequest &request, DatabaseManager* m_db){
    QList<QPair<QByteArray, QByteArray>> headers = request.headers();
    QString authHeader;
    for (const auto &header : headers) {
        if (header.first.toLower() == "authorization") {
            authHeader = QString::fromUtf8(header.second);
            break;
        }
    }
    if (authHeader.isEmpty()) {
        auto response = fail<QJsonObject>("缺少 Authorization 头部");
        return response->toJson();
    }
    QString token;
    if (authHeader.startsWith("Bearer ")) {
        token = authHeader.mid(7);
    } else {
        auto response = fail<QJsonObject>("token错误");
        return response->toJson();
    }
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        try {
            CreateOrderRequest createOrderRequestInfo(jsonObj);
            QJsonObject userInfo = decrypt(token);
            m_db->createOrder(userInfo.value("userID").toInt(),createOrderRequestInfo.getFlightID());
            auto response = success(QString("成功"));
            return response->toJson();
        } catch (const std::invalid_argument& e) {
            auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
            return response->toJson();
        } catch (const std::runtime_error& e) {
            auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
            return response->toJson();
        } catch (const std::exception& e) {
            auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
            return response->toJson();
        }
    }
    auto response = fail<QJsonObject>("failed");
    return response->toJson();
}
