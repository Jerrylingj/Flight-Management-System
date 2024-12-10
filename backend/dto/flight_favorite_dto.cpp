#include "flight_favorite_dto.h"

// 默认构造函数
FavoriteDTO::FavoriteDTO()
    : favoriteId(0), userId(0), flightId(0)
{
}

// 参数化构造函数
FavoriteDTO::FavoriteDTO(int favId, int usrId, int fltId)
    : favoriteId(favId), userId(usrId), flightId(fltId)
{
}

// 转换成 JSON
QJsonObject FavoriteDTO::toJson() const {
    QJsonObject json;
    json["favoriteId"] = favoriteId;
    json["userId"] = userId;
    json["flightId"] = flightId;
    return json;
}
