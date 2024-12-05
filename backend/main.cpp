#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>
#include "database/databasemanager.h"
#include "api/login/login.h"
#include "api/register/register.h"

class HttpServer : public QObject {
public:
    HttpServer(QObject *parent = nullptr) : QObject(parent) {
        m_db = new DatabaseManager();
        m_db->connectToDatabase();
        // 自动创建表，如果没有手动创建过的话
        m_db->createTable();
        // 创建HTTP服务器
        m_httpServer = new QHttpServer(this);

        // 设置路由和处理函数
        m_httpServer->route("/", [](const QHttpServerRequest &request) {
            return "Welcome to Qt HTTP Server!";
        });

        m_httpServer->route("/api/login",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
            return login(request, m_db);
        });

        m_httpServer->route("/api/register",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
            return registerUser(request, m_db);
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
    DatabaseManager* m_db;
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    HttpServer server;

    return app.exec();
}
