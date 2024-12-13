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
#include "api/order/OrderApi.h"
#include "api/favorite/FavoritesApi.h"
#include "aichat/aichat.h"
#include "util/easycrypt.h"

class HttpServer : public QObject {
public:
    HttpServer(QObject *parent = nullptr) : QObject(parent) {
        m_db = new DatabaseManager();
        m_db->connectToDatabase();
        // 自动创建表，如果没有手动创建过的话
        m_db->createTable();

        /*** 测试函数 ***/
        m_db->populateSampleFlights(); // 插入初始航班信息
        m_db->populateSampleOrders();  // 插入初始订单信息

        ai = new AIChat(m_db);

        // 创建HTTP服务器
        m_httpServer = new QHttpServer(this);

        // 设置路由和处理函数
        m_httpServer->route("/", [](const QHttpServerRequest &request) {
            return "Welcome to Qt HTTP Server!";
        });

        /***  API ***/
        /*** users ***/
        // 登录
        m_httpServer->route("/api/login",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
            return login(request, m_db);
        });
        // 注册
        m_httpServer->route("/api/register",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
            return registerUser(request, m_db);
        });

        /*** flight_info ***/
        // 获取所有航班信息
        m_httpServer->route("/api/flights", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request) -> QHttpServerResponse {
            return getFlight(m_db);
        });
        // 获取特定订单
        m_httpServer->route("/api/flights/<arg>", QHttpServerRequest::Method::Get, [this](const int flightId) -> QHttpServerResponse {
            return getFlight(flightId, m_db);
        });

        /*** order ***/
        // 创建订单
        // m_httpServer->route("/api/orders/create",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest &request){
        //     return CreateOrder(request, m_db);
        // });

        m_httpServer->route("/api/orders", QHttpServerRequest::Method::Get,[this](const QHttpServerRequest &request) -> QHttpServerResponse {
            return getOrder(m_db);
        });

        m_httpServer->route("/api/aichat",QHttpServerRequest::Method::Post,[this](const QHttpServerRequest& request){
            return ai->chat(request);
        });

        /*** flight_favorites ***/
        // 添加收藏
        m_httpServer->route("/api/favorites/add", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) {
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();

            qDebug() << json;
            qDebug() << "[调试] 收到 POST 请求 /api/favorites/add";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());

            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的授权令牌"}
                };
            }

            int flightId = json["flightId"].toInt();
            qDebug() << "flightId: " << flightId;
            if (flightId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 flightId";
                return QJsonObject{
                    {"success", false},
                    {"message", "无效或缺少 flightId"}
                };
            }

            qDebug() << "[调试] 提取的航班ID：" << flightId;

            QJsonObject result = addFavorite(m_db, userId, flightId);
            qDebug() << "[调试] 添加收藏结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return result;
        });

        // 取消收藏
        m_httpServer->route("/api/favorites/remove", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) {
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();

            qDebug() << "[调试] 收到 POST 请求 /api/favorites/remove";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());

            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的授权令牌"}
                };
            }

            int flightId = json["flightId"].toInt();
            qDebug() << "flightId: " << flightId;
            if (flightId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 flightId";
                return QJsonObject{
                    {"success", false},
                    {"message", "无效或缺少 flightId"}
                };
            }

            qDebug() << "[调试] 提取的航班ID：" << flightId;

            QJsonObject result = removeFavorite(m_db, userId, flightId);
            qDebug() << "[调试] 取消收藏结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return result;
        });

        // 查询收藏
        m_httpServer->route("/api/favorites", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) {
            qDebug() << "[调试] 收到 POST 请求 /api/favorites";

            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的授权令牌"}
                };
            }

            QJsonObject result = getFavorites(m_db, userId);
            qDebug() << "[调试] 查询收藏结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return result;
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
    AIChat* ai;
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    HttpServer server;

    return app.exec();
}
