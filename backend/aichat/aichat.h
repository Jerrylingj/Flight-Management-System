#ifndef AICHAT_H
#define AICHAT_H

#include <QHttpServer>
#include <QJsonObject>
#include <QJsonArray>
#include "database/databasemanager.h"

class AIChat {
public:
    AIChat(DatabaseManager* m_db);

    // 接收请求并解析数据，调用sendToAI
    QJsonObject chat(const QHttpServerRequest &request);

    // 给ai发信息并获取回复
    QJsonObject sendToAI(QJsonArray& messages);

    QJsonObject useTools(QJsonObject& messages);

    QString callTool(const QString& toolName, const QJsonObject& arg);
private:
    QJsonArray tools;
    QString token;
    QString api_base;
    QString model;
    DatabaseManager* m_db;
};



#endif // AICHAT_H
