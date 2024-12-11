#ifndef FAVORITESAPI_H
#define FAVORITESAPI_H

#include "database/databasemanager.h"
#include <QJsonObject>

// 添加收藏
QJsonObject addFavorite(DatabaseManager* m_db, int userId, int flightId);

// 删除收藏
QJsonObject removeFavorite(DatabaseManager* m_db, int userId, int flightId);

// 查询我的收藏
QJsonObject getFavorites(DatabaseManager* m_db, int userId);

#endif // FAVORITESAPI_H
