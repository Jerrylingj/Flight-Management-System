#include "FavoritesApi.h"
#include <QJsonArray>

// 添加收藏
QJsonObject addFavorite(DatabaseManager* m_db, int userId, int flightId) {
    QJsonObject response;

    if (m_db->addFavorite(userId, flightId)) {
        response["success"] = true;
        response["message"] = "Favorite added successfully";
    } else {
        response["success"] = false;
        response["message"] = "Failed to add favorite";
    }

    return response;
}

// 删除收藏
QJsonObject removeFavorite(DatabaseManager* m_db, int userId, int flightId) {
    QJsonObject response;

    if (m_db->removeFavorite(userId, flightId)) {
        response["success"] = true;
        response["message"] = "Favorite removed successfully";
    } else {
        response["success"] = false;
        response["message"] = "Failed to remove favorite";
    }

    return response;
}

// 查询我的收藏
QJsonObject getFavorites(DatabaseManager* m_db, int userId) {
    QJsonObject response;

    QList<QJsonObject> favoriteFlights = m_db->queryFavorites(userId);
    QJsonArray flights;

    for (const QJsonObject& flight : favoriteFlights) {
        flights.append(flight);
    }

    response["success"] = true;
    response["favorites"] = flights;

    return response;
}

