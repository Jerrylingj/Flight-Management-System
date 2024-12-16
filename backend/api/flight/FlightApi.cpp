#include "FlightApi.h"
#include "dto/response_dto.h"
#include "dto/flight_info_dto.h"
#include <QJsonArray>

QJsonObject getFlight(DatabaseManager* m_db){
    try {
        QJsonArray flights;
        // 从数据库查询航班数据
        m_db->queryFlight(flights);

        // 检查 flights 数组是否为空
        if (flights.isEmpty()) {
            // 如果查询结果为空，返回一个适当的空数据响应
            qDebug() << "查询结果为空";
            return success(QJsonArray())->toJson();  // 返回一个空数组
        }

        // 遍历 flights 数据，确保每个航班数据包含必要字段
        for (int i = 0; i < flights.size(); ++i) {
            QJsonObject flight = flights[i].toObject();

            // 确保每个航班数据包含必要字段
            if (!flight.contains("flightId")) flight["flightId"] = "Unknown";
            if (!flight.contains("flightNumber")) flight["flightNumber"] = "Unknown";
            if (!flight.contains("departureTime")) flight["departureTime"] = "Unknown";
            if (!flight.contains("arrivalTime")) flight["arrivalTime"] = "Unknown";
            if (!flight.contains("departureAirport")) flight["departureAirport"] = "Unknown";
            if (!flight.contains("arrivalAirport")) flight["arrivalAirport"] = "Unknown";
            if (!flight.contains("price")) flight["price"] = 0;
            if (!flight.contains("airlineCompany")) flight["airlineCompany"] = "Unknown";
            if (!flight.contains("status")) flight["status"] = "Unknown";
            if (!flight.contains("isBooked")) flight["isBooked"] = false;
            if (!flight.contains("isFaved")) flight["isFaved"] = false;
            if (!flight.contains("remainingSeats")) flight["remainingSeats"] = 10;

            flights[i] = flight;  // 更新航班数据
        }

        // 返回处理后的航班数据
        auto response = success(flights);
        return response->toJson();

    } catch (const std::runtime_error& e) {
        // 捕获运行时错误并返回错误信息
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    } catch (const std::exception& e) {
        // 捕获其他异常并返回通用的错误信息
        auto response = fail<QJsonObject>("查询失败");
        return response->toJson();
    }
}


QJsonObject getFlight(int flightID,DatabaseManager* m_db){
    try{
        FlightInfo flight;
        m_db->queryFlight(flightID,flight);
        auto response = success(flight.toJson());
        return response->toJson();
    }catch(const std::invalid_argument& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("失败");
        return response->toJson();
    }
}

QJsonObject getFlight(const QHttpServerRequest &request, DatabaseManager* m_db){
    try{
        QJsonArray flights;
        m_db->queryFlight(flights);
        auto response = success(flights);
        return response->toJson();
    }catch(const std::runtime_error& e){
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    }catch(const std::exception& e){
        auto response = fail<QJsonObject>("失败");
        return response->toJson();
    }
}
