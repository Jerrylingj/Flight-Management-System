import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// FlightInfoCard.qml
Item {
    id: flightInfoCard
    width: parent.width
    height: 220

    property string flightNumber
    property int price
    property string departure
    property string destination
    property string departureTime
    property string arrivalTime
    property int remainingSeats
    property bool isBooked: false
    property bool isFaved: false

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
                    text: "航班号: " + flightInfoCard.flightNumber
                    font.pixelSize: 22
                    font.bold: true
                    color: "#2C3E50"
                    Layout.alignment: Qt.AlignLeft
                }

                Text {
                    text: "价格: ￥" + flightInfoCard.price
                    font.pixelSize: 22
                    font.bold: true
                    color: "#E74C3C"
                    Layout.alignment: Qt.AlignRight
                }
            }

            Text {
                text: "出发地: " + flightInfoCard.departure + " - 目的地: " + flightInfoCard.destination
                font.pixelSize: 18
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "时间: " + flightInfoCard.departureTime + " - " + flightInfoCard.arrivalTime
                font.pixelSize: 18
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "剩余座位: " + flightInfoCard.remainingSeats
                font.pixelSize: 18
                color: flightInfoCard.remainingSeats > 0 ? "#27AE60" : "#C0392B"
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignRight

                Button {
                    Layout.minimumWidth: 100
                    height: 40
                    text: flightInfoCard.isBooked ? "取消预订" : "预订"
                    font.pixelSize: 16
                    background: Rectangle {
                        color: flightInfoCard.isBooked ? "#AED6F1" : "#3498DB"
                        radius: 10
                    }
                    onClicked: {
                        flightInfoCard.isBooked = !flightInfoCard.isBooked
                        console.log(flightInfoCard.isBooked ? "取消预订航班: " + flightInfoCard.flightNumber : "预订航班: " + flightInfoCard.flightNumber)
                    }
                }

                Button {
                    Layout.minimumWidth: 100
                    height: 40
                    text: flightInfoCard.isFaved ? "取消收藏" : "收藏"
                    font.pixelSize: 16
                    background: Rectangle {
                        color: flightInfoCard.isFaved ? "#F39C12" : "#F1C40F"
                        radius: 10
                    }
                    onClicked: {
                        flightInfoCard.isFaved = !flightInfoCard.isFaved
                        console.log(flightInfoCard.isFaved ? "取消收藏航班: " + flightInfoCard.flightNumber : "收藏航班: " + flightInfoCard.flightNumber)
                    }
                }
            }
        }
    }
}
