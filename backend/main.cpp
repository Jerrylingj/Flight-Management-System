#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>
#include <QJsonArray>
#include <QObject>
#include "database/databasemanager.h"
#include "api/user/user.h"
#include "api/login/login.h"
#include "api/register/register.h"
#include "api/flight/FlightApi.h"
#include "api/order/CreateOrderApi.h"
#include "api/order/OrderApi.h"
#include "api/favorite/FavoritesApi.h"
#include "aichat/aichat.h"
#include "api/userlist/userlist.h"
#include "util/easycrypt.h"

class HttpServer : public QObject
{
public:
    HttpServer(QObject *parent = nullptr) : QObject(parent)
    {
        m_db = new DatabaseManager();
        m_db->connectToDatabase();
        // 自动创建表，如果没有手动创建过的话
        m_db->createTable();

        /*** 测试函数 ***/
        m_db->populateSampleFlights(); // 插入初始航班信息
        // m_db->populateSampleOrders();  // 插入初始订单信息

        ai = new AIChat(m_db);

        // 创建HTTP服务器
        m_httpServer = new QHttpServer(this);

        // 设置路由和处理函数
        m_httpServer->route("/", [](const QHttpServerRequest &request)
                            { return "Welcome to Qt HTTP Server!"; });

        /***  API ***/
        /*** users ***/
        // 获取用户信息
        m_httpServer->route("/api/user", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request)
                            { return getUserInfo(request, m_db); });
        m_httpServer->route("/api/user", QHttpServerRequest::Method::Put, [this](const QHttpServerRequest &request)
                            { return updateUserInfo(request, m_db); });
        m_httpServer->route("/api/user", QHttpServerRequest::Method::Delete, [this](const QHttpServerRequest &request)
                            { return delUser(request, m_db); });
        // 登录
        m_httpServer->route("/api/login", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            { return login(request, m_db); });
        // 注册
        m_httpServer->route("/api/register", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            { return registerUser(request, m_db); });

        m_httpServer->route("/api/send-code", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            { return sendCode(request); });

        // 获取用户所有信息
        m_httpServer->route("/api/userlist", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            { return getAllUserInfo(request, m_db); });


        /*** flight_info ***/
        // 获取所有航班信息
        m_httpServer->route("/api/flights", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            { return getFlight(m_db); });
        // 动态获取所有航班信息
        m_httpServer->route("/api/flights", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
        {
            qDebug() << "动态查询航班";
            return getFlight(request, m_db);
        });
        // 获取特定航班
        m_httpServer->route("/api/flights/<arg>", QHttpServerRequest::Method::Get, [this](const int flightId) -> QHttpServerResponse
                            { return getFlight(flightId, m_db); });
        // 删除特定航班
        m_httpServer->route("/api/flights/del", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            { return deleteFlight(request, m_db); });
        // 更新航班状态
        m_httpServer->route("/api/flights/update", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            { return updateFlight(request, m_db); });
        // 添加航班
        m_httpServer->route("/api/flights/add", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) {
            return addFlight(request, m_db);
        });

        // 获取相同出发地和目的地的下一个航班
        m_httpServer->route("/api/flights/next", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            {

            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();

            qDebug() << json;
            qDebug() << "[调试] main.cpp - 收到 Post 请求 /api/flights/next";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());
        
            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] main.cpp - 获取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] main.cpp - 无法提取用户ID：" << e.what();
                return QHttpServerResponse(QJsonObject{
                    {"success", false},
                    {"message", "无效的用户token"}
                });
            }
        
            // 从请求参数中提取 flightId，假设 flightId 作为查询参数传递
            int flightId = json["flightId"].toInt();
            qDebug() << "[调试] 提取的flightID：" << flightId;
            if (flightId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 flightId";
                QJsonObject response;
                response["success"] = false;
                response["code"] = 400;
                response["message"] = "无效或缺少 flightId";
                return response;
            }
        
            qDebug() << "[调试] 提取的 flightId：" << flightId;
        
            // 调用 getNextFlight 函数
            QJsonObject result = getNextFlight(flightId, m_db);
            // result["success"] = true;
        
            qDebug() << "[调试] getNextFlight 结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
        
            return QHttpServerResponse(result); });

        /*** order_info ***/

        // 查询当前用户所有订单
        m_httpServer->route("/api/orders", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            {
            qDebug() << "[调试] main.cpp - 收到 Post 请求 /api/orders";

            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] main.cpp - 获取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] main.cpp - 无法提取用户ID：" << e.what();
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的用户token"}
                };
            }

            QJsonObject result = getOrder(m_db, userId);
            qDebug() << "[调试] main.cpp - 当前用户的订单：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return result; });

        // 添加订单
        m_httpServer->route("/api/orders/add", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            {

            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();

            qDebug() << json;
            qDebug() << "[调试] main.cpp - 收到 Post 请求 /api/orders/add";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());

            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                QJsonObject response;
                response["success"] = false;
                response["code"] = 401;
                response["message"] = "无效的授权令牌";
                return response;
            }

            int flightId = json["flightId"].toInt();
            qDebug() << "[调试] 提取的flightID：" << flightId;
            if (flightId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 flightId";
                QJsonObject response;
                response["success"] = false;
                response["code"] = 400;
                response["message"] = "无效或缺少 flightId";
                return response;
            }

            QJsonObject result = createOrder(userId, flightId, m_db); // 调用 payOrder
            qDebug() << "[调试] 支付订单结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return QHttpServerResponse(result); });

        // 支付订单
        m_httpServer->route("/api/orders/pay", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            {
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();
        
            qDebug() << json;
            qDebug() << "[调试] main.cpp - 收到 Post 请求 /api/orders/pay";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());
        
            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                QJsonObject response;
                response["success"] = false;
                response["code"] = 401;
                response["message"] = "无效的授权令牌";
                return response;
            }
        
            int orderId = json["orderId"].toInt();
            qDebug() << "[调试] 提取的orderID：" << orderId;
            if (orderId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 orderId";
                QJsonObject response;
                response["success"] = false;
                response["code"] = 400;
                response["message"] = "无效或缺少 orderId";
                return response;
            }
        
            QJsonObject result = payOrder(orderId, m_db); // 调用 payOrder
            qDebug() << "[调试] 支付订单结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return QHttpServerResponse(result); });

        // 改签订单
        m_httpServer->route("/api/orders/rebook", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            {
            // 解析请求体中的 JSON 数据
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();
        
            qDebug() << json;
            qDebug() << "[调试] main.cpp - 收到 Post 请求 /api/orders/rebook";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());
        
            int userId;
            try {
                // 从请求中提取 userId（假设通过 token 获取）
                userId = getUserID(request);
                qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                QJsonObject response;
                response["success"] = false;
                response["code"] = 401;
                response["message"] = "无效的授权令牌";
                return QHttpServerResponse(response);
            }
        
            // 从请求参数中提取 orderId 和 flightId
            int orderId = json["orderId"].toInt();
            int flightId = json["flightId"].toInt();
            qDebug() << "[调试] 提取的orderID：" << orderId;
            qDebug() << "[调试] 提取的flightID：" << flightId;
        
            // 验证 orderId 和 flightId 是否有效
            if (orderId == 0 || flightId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 orderId 或 flightId";
                QJsonObject response;
                response["success"] = false;
                response["code"] = 400;
                response["message"] = "无效或缺少 orderId 或 flightId";
                return QHttpServerResponse(response);
            }
        
            try {
                // 调用 rebookOrder 函数进行改签
                QJsonObject result = rebookOrder(orderId, flightId, m_db);
        
                qDebug() << "[调试] 改签订单结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
                return QHttpServerResponse(result);
            }
            catch (const std::invalid_argument& e) {
                QJsonObject response;
                response["success"] = false;
                response["code"] = 400;
                response["message"] = QString::fromStdString(e.what());
                return QHttpServerResponse(response);
            }
            catch (const std::runtime_error& e) {
                QJsonObject response;
                response["success"] = false;
                response["code"] = 500;
                response["message"] = QString::fromStdString(e.what());
                return QHttpServerResponse(response);
            }
            catch (const std::exception& e) {
                QJsonObject response;
                response["success"] = false;
                response["code"] = 500;
                response["message"] = "改签失败";
                return QHttpServerResponse(response);
            } });

        // 退签订单
        m_httpServer->route("/api/orders/unpay", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            {
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();
        
            qDebug() << json;
            qDebug() << "[调试] main.cpp - 收到 Post 请求 /api/orders/unpay";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());
        
            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                QJsonObject response;
                response["success"] = false;
                response["code"] = 401;
                response["message"] = "无效的授权令牌";
                return response;
            }
        
            int orderId = json["orderId"].toInt();
            qDebug() << "[调试] 提取的orderID：" << orderId;
            if (orderId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 orderId";
                QJsonObject response;
                response["success"] = false;
                response["code"] = 400;
                response["message"] = "无效或缺少 orderId";
                return response;
            }
        
            QJsonObject result = unpayOrder(orderId, m_db); // 调用 payOrder
            qDebug() << "[调试] 退签订单结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return QHttpServerResponse(result); });

        // 取消支付
        m_httpServer->route("/api/orders/delete", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse
                            {
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();

            qDebug() << json;
            qDebug() << "[调试] main.cpp - 收到 Post 请求 /api/orders/pay";
            qDebug() << "[调试] 请求内容：" << QString::fromUtf8(request.body());

            int userId;
            try {
                userId = getUserID(request);
                qDebug() << "[调试] 提取的 userId：" << userId;
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取 userId：" << e.what();
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的 token"}
                };
            }

            int orderId = json["orderId"].toInt();
            qDebug() << "[调试] 提取的orderID：" << orderId;
            if (orderId == 0) {
                qWarning() << "[警告] 请求体缺少或包含无效的 orderId";
                return QJsonObject{
                    {"success", false},
                    {"message", "无效或缺少 orderId"}
                };
            }

            QJsonObject result = deleteOrder(orderId, m_db);
            qDebug() << "[调试] 删除订单结果：" << QJsonDocument(result).toJson(QJsonDocument::Compact);
            return result; });

        /*** flight_favorites ***/
        // 添加收藏
        m_httpServer->route("/api/favorites/add", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            {
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();

            // 根据token获取用户ID
            int userId;
            try {
                userId = getUserID(request);
                // qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的授权令牌"}
                };
            }

            int flightId = json["flightId"].toInt();
            if (flightId == 0) {
                return QJsonObject{
                    {"success", false},
                    {"message", "无效或缺少 flightId"}
                };
            }

            return addFavorite(m_db, userId, flightId); });

        // 取消收藏
        m_httpServer->route("/api/favorites/remove", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            {
            QJsonDocument body = QJsonDocument::fromJson(request.body());
            QJsonObject json = body.object();

            // 根据token获取用户ID
            int userId;
            try {
                userId = getUserID(request);
                // qDebug() << "[调试] 提取的用户ID：" << userId;
            } catch (std::invalid_argument &e) {
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的授权令牌"}
                };
            }

            int flightId = json["flightId"].toInt();
            if (flightId == 0) {
                return QJsonObject{
                    {"success", false},
                    {"message", "无效或缺少 flightId"}
                };
            }

            return removeFavorite(m_db, userId, flightId); });

        // 查询收藏
        m_httpServer->route("/api/favorites", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            {
            // 根据token获取用户ID
            int userId;
            try {
                userId = getUserID(request);
            } catch (std::invalid_argument &e) {
                qWarning() << "[错误] 无法提取用户ID：" << e.what();
                return QJsonObject{
                    {"success", false},
                    {"message", "无效的授权令牌"}
                };
            }

            return getFavorites(m_db, userId); });

        // AI客服
        m_httpServer->route("/api/aichat", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                            { return ai->chat(request); });

        // 监听端口
        if (m_httpServer->listen(QHostAddress::Any, 8080))
        {
            qDebug() << "HTTP Server is running on port 8080";
        }
        else
        {
            qDebug() << "Failed to start HTTP Server";
        }
    }

private:
    QHttpServer *m_httpServer;
    DatabaseManager *m_db;
    AIChat *ai;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    HttpServer server;

    return app.exec();
}
