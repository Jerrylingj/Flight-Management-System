#ifndef FAVORITEDTO_H
#define FAVORITEDTO_H

#include <QString>
#include <QJsonObject>

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

#endif // FAVORITEDTO_H
