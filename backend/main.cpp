#include <QCoreApplication>
#include <QHttpServer>
#include <QWebSocketServer>
#include <QDebug>
#include "database/databasemanager.h"

class HttpServer : public QObject {
public:
    HttpServer(QObject *parent = nullptr) : QObject(parent) {
        DatabaseManager test = DatabaseManager();
        test.connectToDatabase();
        test.createTable();
        test.insertUser("water","123");
        test.queryUsers();

        // 创建HTTP服务器
        m_httpServer = new QHttpServer(this);

        // 设置路由和处理函数
        m_httpServer->route("/", [](const QHttpServerRequest &request) {
            return "Welcome to Qt HTTP Server!";
        });

        // 监听端口
        if (m_httpServer->listen(QHostAddress::Any, 8080)) {
            qDebug() << "HTTP Server is running on port 8080";
        } else {
            qDebug() << "Failed to start HTTP Server";
        }
    }

private:
    QHttpServer *m_httpServer;
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    HttpServer server;

    return app.exec();
}
