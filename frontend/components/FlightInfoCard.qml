// FlightInfoCard.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: flightInfoCard
    width: parent.width
    height: 220

    property int flightId
    property string flightNumber
    property string departureCity
    property string arrivalCity
    // property string departureTime
    // property string arrivalTime
    property string departureAirport
    property string arrivalAirport
    property double price
    property string airlineCompany
    property string checkinStartTime
    property string checkinEndTime
    property string status
    property bool isBooked: false
    property bool isFaved: false
    property int remainingSeats: 10 // 新增剩余座位属性

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
            spacing: 12

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                spacing: 20

                Text {
                    text: "航班号: " + flightNumber
                    font.pixelSize: 22
                    font.bold: true
                    color: "#2C3E50"
                    Layout.alignment: Qt.AlignLeft
                }

                Text {
                    text: "价格: ￥" + price
                    font.pixelSize: 22
                    font.bold: true
                    color: "#E74C3C"
                    Layout.alignment: Qt.AlignRight
                }
            }

            Text {
                text: "出发地: " + departureCity + " (" + departureAirport + ")"
                font.pixelSize: 18
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "到达地: " + arrivalCity + " (" + arrivalAirport + ")"
                font.pixelSize: 18
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "检票时间: " + checkinStartTime + " - " + checkinEndTime
                font.pixelSize: 18
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "航空公司: " + airlineCompany
                font.pixelSize: 18
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "状态: " + status
                font.pixelSize: 18
                color: status === "On Time" ? "#27AE60" : (status === "Delayed" ? "#F39C12" : "#C0392B")
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "剩余座位: " + remainingSeats
                font.pixelSize: 18
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignRight

                // 预定按钮
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

                // 收藏按钮
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
