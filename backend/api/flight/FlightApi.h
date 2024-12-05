#ifndef FLIGHTAPI_H
#define FLIGHTAPI_H

#include <QObject>
#include <QList>
#include "database/databasemanager.h"
#include "FlightInfo.h"

class FlightAPI : public QObject
{
    Q_OBJECT
public:
    explicit FlightAPI(QObject *parent = nullptr);
    ~FlightAPI();

    // 获取所有航班信息
    QList<FlightInfo> getAllFlights();

    // 根据航班ID获取航班信息
    FlightInfo getFlightById(int flightId);

private:
    DatabaseManager m_dbManager;  // 使用 DatabaseManager 管理数据库
};

#endif // FLIGHTAPI_H
