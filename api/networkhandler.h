#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>

class NetworkHandler : public QObject {
    Q_OBJECT
public:
    enum RequestMethod {
        GET,
        POST,
        // PUT,
        // DELETE
    };

    Q_ENUM(RequestMethod)

    explicit NetworkHandler(QObject *parent = nullptr);

    Q_INVOKABLE void request(
        const QString &url,
        RequestMethod method,
        const QJsonObject &data = QJsonObject()
        );

signals:
    void requestSuccess(const QJsonObject &response);
    void requestFailed(const QString &errorMessage);

private:
    QNetworkAccessManager *m_NetworkHandler;

    QUrl buildUrlWithParams(const QString &baseUrl, const QJsonObject &params);
};
#endif // NETWORKHANDLER_H