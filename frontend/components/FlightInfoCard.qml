import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.LocalStorage 2.0

Item {
    id: flightInfoCard
    width: parent.width
    height: 240  // 设置高度
    anchors.centerIn: parent

    property int flightId
    property string flightNumber
    property string departureTime
    property string arrivalTime
    property string departureAirport
    property string arrivalAirport
    property double price
    property string airlineCompany
    property string status
    property bool isBooked: false
    property bool isFaved: false
    property int remainingSeats: 10

    // 函数：格式化时间为 "hh:mm" 格式
    function formatTime(timeString) {
        var date = new Date(timeString);  // 使用 JavaScript 内置的 Date 对象解析时间字符串
        var hours = date.getHours();  // 获取小时
        var minutes = date.getMinutes();  // 获取分钟

        // 保证小时和分钟都是两位数格式
        return (hours < 10 ? '0' + hours : hours) + ":" + (minutes < 10 ? '0' + minutes : minutes);
    }

    Rectangle {
        width: parent.width - 40
        height: parent.height
        radius: 20
        color: "#FAFAFA"
        border.color: "#DDDDDD"
        border.width: 1
        anchors.horizontalCenter: parent.horizontalCenter

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            // 航班号和价格行
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                spacing: 20

                Text {
                    text: "航班号：" + flightNumber
                    font.pixelSize: 22
                    font.bold: true
                    color: "#2C3E50"
                    Layout.fillWidth: true
                }

                Text {
                    text: "￥" + price.toFixed(2)
                    font.pixelSize: 22
                    font.bold: true
                    color: "#E74C3C"
                    Layout.alignment: Qt.AlignRight
                    Layout.fillWidth: true
                }
            }

            // 出发时间和到达时间行，带箭头
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                spacing: 5

                Text {
                    text: formatTime(departureTime)
                    font.pixelSize: 20
                    font.bold: true
                    color: "#34495E"
                    Layout.fillWidth: true
                }

                // 箭头部分
                Text {
                    text: "→"
                    font.pixelSize: 30
                    color: "#2C3E50"
                    Layout.fillWidth: true
                }

                Text {
                    text: formatTime(arrivalTime)
                    font.pixelSize: 20
                    font.bold: true
                    color: "#34495E"
                    Layout.fillWidth: true
                }
            }

            // 出发机场和到达机场，左对齐
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                spacing: 20

                Text {
                    text: departureAirport
                    font.pixelSize: 16
                    color: "#7F8C8D"
                    Layout.fillWidth: true
                }

                Text {
                    text: arrivalAirport
                    font.pixelSize: 16
                    color: "#7F8C8D"
                    Layout.fillWidth: true
                }
            }

            // 航空公司与状态行
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                spacing: 20

                Text {
                    text: airlineCompany
                    font.pixelSize: 16
                    color: "#34495E"
                    Layout.fillWidth: true
                }

                Rectangle {
                    width: 80
                    height: 30
                    radius: 15
                    color: status === "On Time" ? "#27AE60" : (status === "Delayed" ? "#F39C12" : "#C0392B")
                    border.color: "#DDDDDD"
                    border.width: 1
                    Text {
                        anchors.centerIn: parent
                        text: status
                        color: "white"
                        font.pixelSize: 14
                        font.bold: true
                    }
                }
            }

            // 其他信息行
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                spacing: 20

                Text {
                    text: "剩余座位: " + remainingSeats
                    font.pixelSize: 14
                    color: "#7F8C8D"
                    Layout.fillWidth: true
                }
            }

            // 预定与收藏按钮
            RowLayout {
                Layout.fillWidth: true
                spacing: 20
                Layout.alignment: Qt.AlignRight

                Button {
                    Layout.minimumWidth: 100
                    height: 40
                    text: isBooked ? "取消预订" : "预订"
                    font.pixelSize: 16
                    background: Rectangle {
                        color: isBooked ? "#AED6F1" : "#3498DB"
                        radius: 12
                        gradient: Gradient {
                            GradientStop { position: 0; color: isBooked ? "#89C4F0" : "#3498DB" }
                            GradientStop { position: 1; color: isBooked ? "#AED6F1" : "#2980B9" }
                        }
                    }

                    onClicked: {
                        isBooked = !isBooked
                        console.log(isBooked ? "预订航班: " + flightNumber : "取消预订航班: " + flightNumber)
                    }
                }

                Button {
                    Layout.minimumWidth: 100
                    height: 40
                    text: isFaved ? "取消收藏" : "收藏"
                    font.pixelSize: 16
                    background: Rectangle {
                        color: isFaved ? "#F1C40F" : "#F39C12"
                        radius: 12
                        gradient: Gradient {
                            GradientStop { position: 0; color: isFaved ? "#F1C40F" : "#F8C471" }
                            GradientStop { position: 1; color: isFaved ? "#F9E79F" : "#F39C12" }
                        }
                    }

                    onClicked: {
                        isFaved = !isFaved
                        console.log(isFaved ? "收藏航班: " + flightNumber : "取消收藏航班: " + flightNumber)
                    }
                }
            }
        }
    }
}
