// #include "CreateOrderApi.h"
// #include <QJsonDocument>
// #include "dto/response_dto.h"
// #include "dto/create_order.h"
// #include "util/easycrypt.h"

// QJsonObject CreateOrder(const QHttpServerRequest &request, DatabaseManager* m_db){
//     int userID;
//     try {
//         userID = getUserID(request);
//     } catch (std::invalid_argument& e) {
//         auto response = fail<QJsonObject>(e.what());
//     }
//     QByteArray body = request.body();
//     QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
//     if(!jsonDoc.isNull()&&jsonDoc.isObject()){
//         QJsonObject jsonObj = jsonDoc.object();
//         try {
//             CreateOrderRequest createOrderRequestInfo(jsonObj);
//             m_db->createOrder(userID,createOrderRequestInfo.getFlightID());
//             auto response = success(QString("成功"));
//             return response->toJson();
//         } catch (const std::invalid_argument& e) {
//             auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//             return response->toJson();
//         } catch (const std::runtime_error& e) {
//             auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//             return response->toJson();
//         } catch (const std::exception& e) {
//             auto response = fail<QJsonObject>(QString::fromStdString(e.what()));
//             return response->toJson();
//         }
//     }
//     auto response = fail<QJsonObject>("failed");
//     return response->toJson();
// }
