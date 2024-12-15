#include "networkhandler.h"

#include <QJsonDocument>
#include <QEventLoop>
#include <QNetworkReply>

QJsonObject useNetwork(const QString& targetUrl, const QString& method, const QJsonObject& body, const QString& token){
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();

    QUrl url(targetUrl);

    // 创建网络请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if(!token.isEmpty()){
        request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
    }

    // 发送POST请求
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(body).toJson());

    // 使用事件循环等待响应
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 处理响应
    QJsonObject resp;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        resp = jsonResponse.object();
    } else {
        resp["error"] = reply->errorString();
    }

    // 清理
    reply->deleteLater();
    networkManager->deleteLater();
    return resp;
}
