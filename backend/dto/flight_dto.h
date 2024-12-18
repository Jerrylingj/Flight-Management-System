#ifndef FLIGHT_DTO_H
#define FLIGHT_DTO_H

#include <QHttpServerRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpServer>
#include <QDateTime>
#include <QString>

// 航班信息
class FlightInfo
{
public:
    int flightId;                   // 航班ID
    QString flightNumber;           // 航班号
    QString departureCity;          // 起点市
    QString arrivalCity;            // 终点市
    QDateTime departureTime;        // 出发时间
    QDateTime arrivalTime;          // 到达时间
    QString departureAirport;       // 起点机场
    QString arrivalAirport;         // 终点机场
    QDateTime checkinStartTime;     // 检票开始时间
    QDateTime checkinEndTime;       // 检票结束时间
    double price;                   // 价格
    QString airlineCompany;         // 航空公司
    QString status;                 // 航班状态（正常、延误……）

    // 构造函数
    FlightInfo();
    FlightInfo(int id, const QString &flightNum, const QString &depCity, const QString &arrCity,const QDateTime &depTime, const QDateTime &arrTime,
               const QString &depAirport, const QString &arrAirport, const QDateTime &checkinStart,
               const QDateTime &checkinEnd, double pr, const QString &airline, const QString &status);

    // 转换成 JSON 或其他格式等功能
    QJsonObject toJson() const;
};

// 航班更新
class FlightUpdateDTO {
public:
    QString authCode;
    int flightId;
    QString flightNumber;
    QString departureCity;
    QString arrivalCity;
    QDateTime departureTime;
    QDateTime arrivalTime;
    double price;
    QString departureAirport;
    QString arrivalAirport;
    QString airlineCompany;
    QDateTime checkinStartTime;
    QDateTime checkinEndTime;
    QString status;

    FlightUpdateDTO(const QHttpServerRequest &request);

    bool isValid() const;
};

// 航班删除
class FlightDelDTO
{
public:
    // 从请求中解析数据
    FlightDelDTO(const QHttpServerRequest &request);

    QString getAuthCode() const;

    int getFlightId() const;

private:
    QString authCode;
    int flightId;
};

// 航班收藏
class FavoriteDTO
{
public:
    int favoriteId;         // 收藏记录 ID
    int userId;             // 用户 ID
    int flightId;           // 航班 ID

    // 构造函数
    FavoriteDTO();
    FavoriteDTO(int favId, int usrId, int fltId);

    // 转换成 JSON 格式
    QJsonObject toJson() const;
};


#endif // FLIGHT_DTO_H
