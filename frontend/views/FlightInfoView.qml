import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// FlightInfoView.qml
Page {
    id: flightInfoView

    property string viewName: "航班信息"

    // 模拟出来的数据
    property var flightData: [
            { "flightNumber": "CA123", "price": 1800, "departure": "北京", "destination": "上海", "departureTime": "08:30", "arrivalTime": "11:00", "remainingSeats": 5, isBooked: true, isFaved: true },
            { "flightNumber": "MU456", "price": 2000, "departure": "北京", "destination": "上海", "departureTime": "09:00", "arrivalTime": "11:30", "remainingSeats": 3, isBooked: false, isFaved: false },
            { "flightNumber": "HU789", "price": 1500, "departure": "北京", "destination": "上海", "departureTime": "10:00", "arrivalTime": "12:30", "remainingSeats": 10, isBooked:true, isFaved: false },
            { "flightNumber": "CA321", "price": 2200, "departure": "北京", "destination": "广州", "departureTime": "07:00", "arrivalTime": "09:30", "remainingSeats": 2, isBooked: false, isFaved: false },
            { "flightNumber": "MU654", "price": 1800, "departure": "上海", "destination": "北京", "departureTime": "12:00", "arrivalTime": "14:30", "remainingSeats": 7, isBooked: false, isFaved: true }
        ]

    // // 每个航班信息
    // property var filteredFlights: flightData


    ScrollView {
        width: parent.width
        height: parent.height


        ColumnLayout {
            id: flightInfoList
            spacing: 20
            anchors.margins: 20

            // 筛选条件
            Rectangle {
                color: "#ffffff"
                radius: 8
                border.color: "#ddd"
                border.width: 1
                width: parent.width - 40// 留出一些空白，防止边界超出屏幕
                height: 40
                Layout.alignment: Qt.AlignHCenter
                anchors.margins: 10

                GridLayout {
                    columns: 4
                    columnSpacing: 10
                    rowSpacing: 10
                    anchors.fill: parent
                    anchors.margins: 15

                    TextField {
                        id: departureField
                        placeholderText: "出发地"
                        Layout.maximumWidth: 100
                        font.pixelSize: 14
                        background: Rectangle {
                            border.width: 1
                            color: "#f2f2f2"
                            radius: 8
                        }
                        padding: 8
                    }

                    Text {
                        text: "—"
                        font.pixelSize: 14
                    }

                    TextField {
                        id: destinationField
                        placeholderText: "目的地"
                        Layout.maximumWidth: 100
                        font.pixelSize: 14
                        background: Rectangle {
                            border.width: 1
                            color: "#f2f2f2"
                            radius: 8
                        }
                        padding: 8
                    }

                    ComboBox {
                        id: sortMethodComboBox
                        model: ["按时间排序", "按价格排序", "按座位排序"]
                        width: parent.width / 5
                        font.pixelSize: 14
                        background: Rectangle {
                            border.width: 1
                            color: "#f2f2f2"
                            radius: 1
                        }
                        onActivated: updateFilter()
                    }

                    Button {
                        text: "筛选"
                        Layout.maximumWidth: 80
                        height: 40
                        font.pixelSize: 14
                        background: Rectangle {
                            color: "#1976d2"
                            radius: 8
                        }
                        onClicked: updateFilter()
                    }
                }
            }


            // Sample flight information cards
            Repeater {
                model: 5 // 假设展示5条航班信息，实际项目中可以绑定数据模型
                Loader {
                    source: "../components/FlightInfoCard.qml"
                    property var flightInfo: flightData[index]
                    onLoaded: {
                        item.flightNumber = flightInfo.flightNumber;
                        item.price = flightInfo.price;
                        item.departure = flightInfo.departure;
                        item.destination = flightInfo.destination;
                        item.departureTime = flightInfo.departureTime;
                        item.arrivalTime = flightInfo.arrivalTime;
                        item.remainingSeats = flightInfo.remainingSeats;
                        item.isBooked = flightInfo.isBooked;
                        item.isFaved = flightInfo.isFaved;
                    }
                }
            }
        }
    }

    // 筛选和排序更新函数
    function updateFilter() {
        var searchDeparture = departureField.text;
        var searchDestination = destinationField.text;
        var sortMethod = sortMethodComboBox.currentIndex;

        filteredFlights = flightData.filter(function(flight) {
            return flight.departure.includes(searchDeparture) && flight.destination.includes(searchDestination);
        });

        switch (sortMethod) {
            case 0: // 按时间排序
                filteredFlights.sort(function(a, b) {
                    return new Date("1970-01-01T" + a.departureTime) - new Date("1970-01-01T" + b.departureTime);
                });
                break;
            case 1: // 按价格排序
                filteredFlights.sort(function(a, b) {
                    return a.price - b.price;
                });
                break;
            case 2: // 按剩余座位排序
                filteredFlights.sort(function(a, b) {
                    return a.remainingSeats - b.remainingSeats;
                });
                break;
        }
    }
}
