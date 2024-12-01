import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: flightInfoView

    property string viewName: "航班信息"

    // 模拟数据
    property var flightData: [
        { "flightNumber": "CA123", "price": 1800, "departure": "北京", "destination": "上海", "departureTime": "08:30", "arrivalTime": "11:00", "remainingSeats": 5, isBooked: true, isFaved: true },
        { "flightNumber": "MU456", "price": 2000, "departure": "北京", "destination": "上海", "departureTime": "09:00", "arrivalTime": "11:30", "remainingSeats": 3, isBooked: false, isFaved: false },
        { "flightNumber": "HU789", "price": 1500, "departure": "北京", "destination": "上海", "departureTime": "10:00", "arrivalTime": "12:30", "remainingSeats": 10, isBooked: true, isFaved: false },
        { "flightNumber": "CA321", "price": 2200, "departure": "北京", "destination": "广州", "departureTime": "07:00", "arrivalTime": "09:30", "remainingSeats": 2, isBooked: false, isFaved: false },
        { "flightNumber": "MU654", "price": 1800, "departure": "上海", "destination": "北京", "departureTime": "12:00", "arrivalTime": "14:30", "remainingSeats": 7, isBooked: false, isFaved: true }
    ]

    // 跟踪当前的排序方法和排序方向
    property int sortMethod: 0  // 默认按时间排序
    property bool isAscending: true  // 默认升序

    ScrollView {
        width: parent.width
        height: parent.height

        ColumnLayout {
            id: flightInfoList
            spacing: 20
            anchors.margins: 20
            width: parent.width

            // 筛选条件
            Rectangle {
                color: "#FAFAFA"
                radius: 12
                border.color: "#DDDDDD"
                border.width: 1
                width: parent.width
                height: 80
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop  // 水平居中，顶部对齐
                Layout.topMargin: 10  // 设置上边距为 20 像素

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    Layout.fillWidth: true  // 确保 RowLayout 填充整个可用宽度
                    spacing: 10

                    // 出发地输入框
                    TextField {
                        id: departureField
                        placeholderText: "出发地"
                        Layout.preferredWidth: 80  // 使用 preferredWidth 而不是 maximumWidth
                        font.pixelSize: 16
                        background: Rectangle {
                            color: "#F9F9F9"
                            radius: 10
                            border.color: "#DDDDDD"
                            border.width: 1
                        }
                        padding: 12
                        color: "#333"
                        Layout.alignment: Qt.AlignVCenter  // 确保垂直居中
                    }

                    // 分隔符（“—”）
                    Text {
                        text: "—"
                        font.pixelSize: 16
                        color: "#333"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        Layout.preferredWidth: 15  // 给分隔符一个合理的宽度
                        Layout.alignment: Qt.AlignVCenter
                    }

                    // 目的地输入框
                    TextField {
                        id: destinationField
                        placeholderText: "目的地"
                        Layout.preferredWidth: 80  // 使用 preferredWidth 而不是 maximumWidth
                        font.pixelSize: 16
                        background: Rectangle {
                            color: "#F9F9F9"
                            radius: 10
                            border.color: "#DDDDDD"
                            border.width: 1
                        }
                        padding: 12
                        color: "#333"
                        Layout.alignment: Qt.AlignVCenter
                    }

                    // 排序选择框
                    ComboBox {
                        id: sortMethodComboBox
                        model: [
                            { label: "时间", icon: isAscending ? "↑" : "↓" },
                            { label: "价格", icon: isAscending ? "↑" : "↓" },
                            { label: "座位", icon: isAscending ? "↑" : "↓" }
                        ]
                        Layout.preferredWidth: 55  // 使用 preferredWidth 而不是 maximumWidth
                        font.pixelSize: 16
                        background: Rectangle {
                            color: "#F9F9F9"
                            radius: 10
                            border.color: "#DDDDDD"
                            border.width: 1
                        }
                        textRole: "label"  // 显示 label 属性作为文本
                        onActivated: {
                            sortMethod = currentIndex;
                            toggleSortOrder();
                            updateFilter();
                        }
                        Layout.alignment: Qt.AlignVCenter
                    }

                    // 筛选按钮
                    Button {
                        text: "筛选"
                        font.pixelSize: 16
                        Layout.preferredWidth: 80
                        height: 40
                        background: Rectangle {
                            color: "#007AFF"
                            radius: 12
                            gradient: Gradient {
                                GradientStop { position: 0; color: "#3498DB" }
                                GradientStop { position: 1; color: "#2980B9" }
                            }
                        }
                        onClicked: updateFilter()
                        Layout.alignment: Qt.AlignVCenter
                    }
                }
            }

            // 动态展示航班信息
            Repeater {
                model: filteredFlights // 绑定动态筛选后的航班列表
                Loader {
                    width: parent.width
                    source: "../components/FlightInfoCard.qml"
                    property var flightInfo: modelData // 传递每条航班的数据
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

        // 动态筛选航班数据
        filteredFlights = flightData.filter(function(flight) {
            return flight.departure.includes(searchDeparture) && flight.destination.includes(searchDestination);
        });

        // 排序功能
        switch (sortMethod) {
            case 0: // 按时间排序
                filteredFlights.sort(function(a, b) {
                    var timeA = a.departureTime.split(":");
                    var timeB = b.departureTime.split(":");
                    var dateA = new Date(1970, 0, 1, timeA[0], timeA[1]);
                    var dateB = new Date(1970, 0, 1, timeB[0], timeB[1]);
                    return isAscending ? dateA - dateB : dateB - dateA;
                });
                break;
            case 1: // 按价格排序
                filteredFlights.sort(function(a, b) {
                    return isAscending ? a.price - b.price : b.price - a.price;
                });
                break;
            case 2: // 按剩余座位排序
                filteredFlights.sort(function(a, b) {
                    return isAscending ? a.remainingSeats - b.remainingSeats : b.remainingSeats - a.remainingSeats;
                });
                break;
        }

        filteredFlights = filteredFlights.slice();  // 重新赋值给数组，触发数据更新
    }

    // 切换排序顺序
    function toggleSortOrder() {
        isAscending = !isAscending;
        sortMethodComboBox.model = [
            { label: "时间", src: isAscending ? "https://img.icons8.com/?size=100&id=122841&format=png&color=000000" : "https://img.icons8.com/?size=100&id=h2X0Cy3sU70i&format=png&color=000000" },
            { label: "价格", src: isAscending ? "https://img.icons8.com/?size=100&id=122841&format=png&color=000000" : "https://img.icons8.com/?size=100&id=h2X0Cy3sU70i&format=png&color=000000" },
            { label: "座位", src: isAscending ? "https://img.icons8.com/?size=100&id=122841&format=png&color=000000" : "https://img.icons8.com/?size=100&id=h2X0Cy3sU70i&format=png&color=000000" }
        ];
    }

    // 初始化时显示所有航班
    property var filteredFlights: flightData
}
