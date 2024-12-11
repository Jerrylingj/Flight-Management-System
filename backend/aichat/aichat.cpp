#include "aichat.h"

#include <QFile>
#include <QJsonDocument>
#include "dto/response_dto.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>

AIChat::AIChat(DatabaseManager* m_db){
    this->m_db = m_db;
    QFile configFile(":/config/ai.json");
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = configFile.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject config = jsonDoc.object();

            // 检查配置项是否存在
            if (config.contains("token") && config.contains("tools") && config.contains("api_base") && config.contains("model")) {
                token = config.value("token").toString();
                tools = config.value("tools").toArray();
                api_base = config.value("api_base").toString();
                model = config.value("model").toString();
            } else {
                qDebug() << "Error: 初始化ai失败";
            }
        } else {
            qDebug() << "Error: Invalid JSON format";
        }

        configFile.close();
    } else {
        qDebug() << "Error: Could not open config file" << configFile.errorString();
    }
}

QJsonObject AIChat::chat(const QHttpServerRequest& request){
    QByteArray body = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    if(!jsonDoc.isNull()&&jsonDoc.isObject()){
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray messages = jsonObj.value("messages").toArray();
        QJsonObject answer = sendToAI(messages);
        if(answer.value("error").isNull()||answer.value("error").isUndefined()){
            auto response = success(answer);
            return response->toJson();
        }else{
            auto response = fail<QJsonObject>(answer.value("error").toString());
            return response->toJson();
        }

    }
    auto response = fail<QJsonObject>("failed");
    return response->toJson();
}

QJsonObject AIChat::sendToAI(QJsonArray& messages){
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();

    QUrl url(api_base);

    // 创建网络请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    // 准备JSON请求体
    QJsonObject requestBody;
    requestBody["model"] = model;
    requestBody["messages"] = messages;
    requestBody["tools"] = tools;

    // 发送POST请求
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(requestBody).toJson());

    // 使用事件循环等待响应
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 处理响应
    QJsonObject message;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        message = jsonResponse.object().value("choices").toArray()[0].toObject().value("message").toObject();
        if(message.value("content").isUndefined()){
            messages.append(message);
            QJsonObject result = useTools(message);
            messages.append(result);
            message = sendToAI(messages);
        }
    } else {
        message["error"] = reply->errorString();
    }

    // 清理
    reply->deleteLater();
    networkManager->deleteLater();
    return message;
}

QJsonObject AIChat::useTools(QJsonObject& message){
    QJsonObject tool_call = message.value("tool_calls").toArray()[0].toObject();
    QJsonObject function = tool_call.value("function").toObject();
    QString argumentsStr = function.value("arguments").toString();
    QJsonDocument doc = QJsonDocument::fromJson(argumentsStr.toUtf8());
    QJsonObject newMessage;
    newMessage["role"] = "tool";
    newMessage["tool_call_id"] = tool_call.value("id").toString();
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject argumentsObj = doc.object();
        qDebug() << argumentsObj;
        QString result = callTool(function.value("name").toString(), argumentsObj);
        newMessage["content"] = result;
    }else{
        newMessage["content"] = "失败了，请确保参数正确";
    }
    return newMessage;

}

QString AIChat::callTool(const QString& toolName, const QJsonObject& arg){
    if(toolName == "get_flight"){
        QJsonArray flights;
        qDebug() << arg;
        if(arg.contains("arrivalCity")&&arg.contains("departureCity")){
            try{
                m_db->queryFlight(flights, arg.value("departureCity").toString(), arg.value("arrivalCity").toString());
            }catch(std::runtime_error& e){
                return e.what();
            }
            if(flights.isEmpty()){
                return "没有这样的航班";
            }else{
                QJsonDocument doc(flights);
                QString jsonString = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
                return jsonString;
            }
        }else{
            return "缺少必要的参数";
        }
    }
    return "这个函数不存在";
}
