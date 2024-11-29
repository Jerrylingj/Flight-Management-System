import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// OrdersView.qml
Page {
    id: ordersView

    property string viewName: '全部订单'

    ScrollView {
        width: parent.width
        height: parent.height

        ColumnLayout {
            id: ordersList
            spacing: 20
            anchors.margins: 20

            // 示例订单信息数据
            Repeater {
                model: 5 // 假设展示5条订单信息，实际项目中可以绑定数据模型
                Loader {
                    source: "../components/OrderInfoCard.qml"
                    property var orderInfo: {
                        "flightNumber": "CA123" + index,
                        "departureTime": "08:00",
                        "arrivalTime": "10:00",
                        "departureCity": "北京",
                        "arrivalCity": "上海",
                        "checkInTime": "07:00"
                    }
                    onLoaded: {
                        item.flightNumber = orderInfo.flightNumber;
                        item.departure = orderInfo.departureCity;
                        item.destination = orderInfo.arrivalCity;
                        item.departureTime = orderInfo.departureTime;
                        item.arrivalTime = orderInfo.arrivalTime;
                        item.checkInTime = orderInfo.checkInTime;
                    }
                }
            }
        }
    }
}
