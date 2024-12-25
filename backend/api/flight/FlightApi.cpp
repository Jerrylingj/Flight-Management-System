#include "FlightApi.h"
#include "dto/response_dto.h"
#include "dto/flight_dto.h"
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
    try {
        // 解析请求参数
        FlightInfoDTO flightInfoDTO(request);
        int offset = flightInfoDTO.offset;
        int limit = flightInfoDTO.limit;

        QJsonArray flights;
        m_db->queryFlight(flights, offset, limit);

        if (flights.isEmpty()) {
            qDebug() << "查询结果为空";
            return success(QJsonArray())->toJson(); // 返回空数组
        }

        auto response = success(flights);
        return response->toJson();
    } catch (const std::runtime_error &e) {
        auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
        return response->toJson();
    } catch (const std::exception &e) {
        auto response = fail<QJsonObject>("查询失败");
        return response->toJson();
    }
}

// 用于改签：获取和flightId所指向的航班相同出发地和目的地的、出发时间晚于flightId所指向的航班，在当天、明天或后天出发的所有航班，转化为QJsonObject，返还给Controller层
QJsonObject getNextFlight(int flightId, DatabaseManager* m_db){
    QJsonObject response;

    if (!m_db) {
        response["success"] = false;
        response["code"] = 500;
        response["message"] = "获取后续航班失败：DatabaseManager 指针为空";
        return response;
    }

    try {
        QJsonArray flights;
        // 查询符合条件的所有航班
        m_db->queryNextFlights(flightId, flights); // 注意：需要在DatabaseManager中实现新的queryNextFlights方法

        response["success"] = true;
        response["code"] = 200;
        response["data"] = flights;
        response["message"] = flights.isEmpty() ? "未找到符合条件的航班" : "操作成功";
        return response;

    } catch(const std::runtime_error& e){
        response["success"] = false;
        response["code"] = 500;
        response["message"] = QString::fromStdString(e.what());
        return response;
    } catch(const std::exception& e){
        response["success"] = false;
        response["code"] = 500;
        response["message"] = "获取后续航班失败";
        return response;
    }
}


QJsonObject deleteFlight(const QHttpServerRequest &request, DatabaseManager *m_db) {
    try {
        // 使用 FlightDelDTO 解析请求
        FlightDelDTO flightDelDTO(request);

        // 校验 authCode
        if (flightDelDTO.getAuthCode() != "123") {
            throw std::invalid_argument("Invalid authCode");
        }

        // 调用数据库删除函数
        if (!m_db->deleteFlight(flightDelDTO.getFlightId())) {
            throw std::runtime_error("Failed to delete flight");
        }

        // 返回成功响应
        QJsonObject response;
        response["success"] = true;
        response["message"] = "Flight deleted successfully";
        return response;

    } catch (const std::invalid_argument &e) {
        qWarning() << "Invalid argument error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = QString("Invalid input: %1").arg(e.what());
        return response;

    } catch (const std::runtime_error &e) {
        qWarning() << "Runtime error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = QString("Error: %1").arg(e.what());
        return response;

    } catch (const std::exception &e) {
        qWarning() << "Unexpected error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = "An unexpected error occurred";
        return response;
    }
}


QJsonObject addFlight(const QHttpServerRequest &request, DatabaseManager *m_db) {
    try {
        // 使用 FlightAddDTO 解析请求
        FlightAddDTO flightAddDTO(request);

        // 校验授权码
        if (flightAddDTO.authCode != "123") {
            throw std::invalid_argument("Invalid authCode");
        }

        // 调用数据库插入航班函数
        bool success = m_db->insertFlight(
            flightAddDTO.flightNumber,
            flightAddDTO.departureCity,
            flightAddDTO.arrivalCity,
            flightAddDTO.departureTime,
            flightAddDTO.arrivalTime,
            flightAddDTO.price,
            flightAddDTO.departureAirport,
            flightAddDTO.arrivalAirport,
            flightAddDTO.airlineCompany,
            flightAddDTO.checkinStartTime,
            flightAddDTO.checkinEndTime,
            flightAddDTO.status
            );

        if (!success) {
            throw std::runtime_error("Failed to add flight");
        }

        // 返回成功响应
        QJsonObject response;
        response["success"] = true;
        response["message"] = "Flight added successfully";
        return response;

    } catch (const std::invalid_argument &e) {
        qWarning() << "Invalid argument error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = QString("Invalid input: %1").arg(e.what());
        return response;

    } catch (const std::runtime_error &e) {
        qWarning() << "Runtime error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = QString("Error: %1").arg(e.what());
        return response;

    } catch (const std::exception &e) {
        qWarning() << "Unexpected error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = "An unexpected error occurred";
        return response;
    }
}
QJsonObject updateFlight(const QHttpServerRequest &request, DatabaseManager *m_db) {
    try {
        // 使用 FlightUpdateDTO 解析请求
        FlightUpdateDTO flightUpdateDTO(request);

        // 校验 authCode
        if (flightUpdateDTO.authCode != "123") {
            throw std::invalid_argument("Invalid authCode");
        }

        // 调用数据库更新函数
        bool success = m_db->updateFlightInfo(
            flightUpdateDTO.flightId,
            flightUpdateDTO.flightNumber,
            flightUpdateDTO.departureCity,
            flightUpdateDTO.arrivalCity,
            flightUpdateDTO.departureTime,
            flightUpdateDTO.arrivalTime,
            flightUpdateDTO.price,
            flightUpdateDTO.departureAirport,
            flightUpdateDTO.arrivalAirport,
            flightUpdateDTO.airlineCompany,
            flightUpdateDTO.checkinStartTime,
            flightUpdateDTO.checkinEndTime,
            flightUpdateDTO.status
            );

        if (!success) {
            throw std::runtime_error("Failed to update flight information");
        }

        // 返回成功响应
        QJsonObject response;
        response["success"] = true;
        response["message"] = "Flight updated successfully";
        return response;

    } catch (const std::invalid_argument &e) {
        qWarning() << "Invalid argument error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = QString("Invalid input: %1").arg(e.what());
        return response;

    } catch (const std::runtime_error &e) {
        qWarning() << "Runtime error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = QString("Error: %1").arg(e.what());
        return response;

    } catch (const std::exception &e) {
        qWarning() << "Unexpected error:" << e.what();
        QJsonObject response;
        response["success"] = false;
        response["message"] = "An unexpected error occurred";
        return response;
    }
}
