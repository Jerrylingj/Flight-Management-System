#include "FlightApi.h"
#include "dto/response_dto.h"
#include "dto/flight_info_dto.h"
#include <QJsonArray>

QJsonObject getFlight(DatabaseManager* m_db){
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
