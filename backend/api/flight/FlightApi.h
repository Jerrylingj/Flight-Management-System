#ifndef FLIGHTAPI_H
#define FLIGHTAPI_H

#include <QObject>
#include <memory>
#include "dto/response_dto.h"
#include "FlightInfo.h"
#include "database/databasemanager.h"

class FlightAPI : public QObject
{
    Q_OBJECT
public:
    explicit FlightAPI(QObject *parent = nullptr);

    // 获取所有航班信息
    std::shared_ptr<ResponseDTO<QList<FlightInfo>>> getAllFlights();

    // 添加航班
    std::shared_ptr<ResponseDTO<QString>> addFlight(const FlightInfo &flight);

    // 更新航班
    std::shared_ptr<ResponseDTO<QString>> updateFlight(int flightId, const FlightInfo &flight);

    // 删除航班
    std::shared_ptr<ResponseDTO<QString>> deleteFlight(int flightId);

private:
    DatabaseManager m_dbManager;  // 数据库管理类
};

#endif // FLIGHTAPI_H
