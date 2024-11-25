import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// FlightInfoView.qml
Page {
    id: flightInfoView

    property string viewName: "航班信息"

    ScrollView {
        width: parent.width
        height: parent.height

        ColumnLayout {
            id: flightInfoList
            spacing: 20
            anchors.margins: 20

            // Sample flight information cards
            Repeater {
                model: 5 // 假设展示5条航班信息，实际项目中可以绑定数据模型
                Loader {
                    source: "../components/FlightInfoCard.qml"
                    property var flightInfo: {
                        "flightNumber": "CA123" + index,
                        "price": 1800 + index * 100,
                        "departure": "北京",
                        "destination": "上海",
                        "departureTime": "08:30",
                        "arrivalTime": "11:00",
                        "remainingSeats": 5 - index
                    }
                    onLoaded: {
                        item.flightNumber = flightInfo.flightNumber;
                        item.price = flightInfo.price;
                        item.departure = flightInfo.departure;
                        item.destination = flightInfo.destination;
                        item.departureTime = flightInfo.departureTime;
                        item.arrivalTime = flightInfo.arrivalTime;
                        item.remainingSeats = flightInfo.remainingSeats;
                    }
                }
            }
        }
    }
}
