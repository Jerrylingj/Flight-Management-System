#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>
#include <QJsonArray>
#include <QObject>
#include "database/databasemanager.h"
#include "api/login/login.h"
#include "api/register/register.h"
#include "api/flight/FlightAPI.h"

class HttpServer : public QObject {
public:
    HttpServer(QObject *parent = nullptr) : QObject(parent) {
        m_db = new DatabaseManager();
        m_db->connectToDatabase();
        // 自动创建表，如果没有手动创建过的话
        m_db->createTable();
        // 创建HTTP服务器
        m_httpServer = new QHttpServer(this);
        // 创建FlightAPI实例并传入数据库连接
        m_flightAPI = new FlightAPI(m_db, this);  // 修改这里，传入m_db

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
        m_httpServer->route("/api/flights", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request) -> QByteArray {
            // 获取所有航班信息
            QList<FlightInfo> flights = m_flightAPI->getAllFlights();

            // 将航班信息转成JSON返回
            QJsonArray flightArray;
            for (const FlightInfo &flight : flights) {
                QJsonObject flightObject;
                flightObject["flight_id"] = flight.flightId;
                flightObject["flight_number"] = flight.flightNumber;
                flightObject["departure_city"] = flight.departureCity;
                flightObject["arrival_city"] = flight.arrivalCity;
                flightObject["departure_airport"] = flight.departureAirport;
                flightObject["arrival_airport"] = flight.arrivalAirport;
                flightObject["checkin_start_time"] = flight.checkinStartTime.toString();
                flightObject["checkin_end_time"] = flight.checkinEndTime.toString();
                flightObject["price"] = flight.price;
                flightObject["airline_company"] = flight.airlineCompany;
                flightObject["status"] = flight.status;

                flightArray.append(flightObject);
            }

            // 返回所有航班信息的JSON数组
            return QJsonDocument(flightArray).toJson();
        });

        // 根据航班ID获取航班信息的API路由
        m_httpServer->route("/api/flights/{id}", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request) -> QByteArray {
            // 获取路径中的航班ID，使用 URL 的 path 来提取
            QUrl url = request.url();
            QString path = url.path();

            // 提取 {id} 部分
            QStringList pathParts = path.split("/");

            if (pathParts.size() < 3) {
                return "Invalid URL format";  // 如果路径不包含航班ID
            }

            bool ok;
            int flightId = pathParts[2].toInt(&ok);  // 获取路径中的航班ID
            if (!ok) {
                return "Invalid flight ID";  // 返回错误信息
            }

            // 根据航班ID获取航班信息
            FlightInfo flight = m_flightAPI->getFlightById(flightId);

            // 如果没有找到航班信息
            if (flight.flightId == 0) {
                return "Flight not found";  // 返回未找到航班的提示
            }

            // 将航班信息转成JSON返回
            QJsonObject flightObject;
            flightObject["flight_id"] = flight.flightId;
            flightObject["flight_number"] = flight.flightNumber;
            flightObject["departure_city"] = flight.departureCity;
            flightObject["arrival_city"] = flight.arrivalCity;
            flightObject["departure_airport"] = flight.departureAirport;
            flightObject["arrival_airport"] = flight.arrivalAirport;
            flightObject["checkin_start_time"] = flight.checkinStartTime.toString();
            flightObject["checkin_end_time"] = flight.checkinEndTime.toString();
            flightObject["price"] = flight.price;
            flightObject["airline_company"] = flight.airlineCompany;
            flightObject["status"] = flight.status;

            // 返回单个航班信息的JSON对象
            return QJsonDocument(flightObject).toJson();
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
    FlightAPI* m_flightAPI;
    DatabaseManager* m_db;
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    HttpServer server;

    return app.exec();
}
