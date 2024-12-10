#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>
#include <QJsonArray>
#include <QObject>
#include "database/databasemanager.h"
#include "api/login/login.h"
#include "api/register/register.h"
#include "api/flight/FlightApi.h"
#include "api/order/CreateOrderApi.h"

class HttpServer : public QObject {
public:
    HttpServer(QObject *parent = nullptr) : QObject(parent) {
        m_db = new DatabaseManager();
        m_db->connectToDatabase();
        // 自动创建表，如果没有手动创建过的话
        m_db->createTable();

        /*** 测试函数 ***/
        m_db->populateSampleFlights(); // 插入航班初始航班信息

        // 创建HTTP服务器
        m_httpServer = new QHttpServer(this);

        // 设置路由和处理函数
        m_httpServer->route("/", [](const QHttpServerRequest &request) {
            return "Welcome to Qt HTTP Server!";
        });

        /***  API ***/
        // 登录
        m_httpServer->route("/api/login",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
            return login(request, m_db);
        });

        // 注册
        m_httpServer->route("/api/register",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
            return registerUser(request, m_db);
        });

        // 获取所有航班信息的API路由
        m_httpServer->route("/api/flights", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request) -> QHttpServerResponse {
            return getFlight(m_db);
        });


        // 根据航班ID获取航班信息的API路由
        // 获取特定订单
        m_httpServer->route("/api/flights/<arg>", QHttpServerRequest::Method::Get, [this](const int flightId) -> QHttpServerResponse {
            return getFlight(flightId, m_db);
        });
        // 获取全部订单
        m_httpServer->route("/api/create-order",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
            return CreateOrder(request, m_db);
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
