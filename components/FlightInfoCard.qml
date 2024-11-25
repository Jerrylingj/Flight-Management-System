import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// FlightInfoCard.qml
Item {
    id: flightInfoCard
    width: parent.width
    height: 200

    property string flightNumber
    property int price
    property string departure
    property string destination
    property string departureTime
    property string arrivalTime
    property int remainingSeats

    Rectangle {
        width: parent.width - 40
        height: parent.height
        radius: 20
        color: "#FAFAFA"
        border.color: "#CCCCCC"
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
                    font.pixelSize: 24
                    font.bold: true
                    color: "#2C3E50"
                    Layout.alignment: Qt.AlignLeft
                }

                Text {
                    text: "价格: ￥" + flightInfoCard.price
                    font.pixelSize: 24
                    font.bold: true
                    color: "#E74C3C"
                    Layout.alignment: Qt.AlignRight
                }
            }

            Text {
                text: "出发地: " + flightInfoCard.departure + " - 目的地: " + flightInfoCard.destination
                font.pixelSize: 20
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "时间: " + flightInfoCard.departureTime + " - " + flightInfoCard.arrivalTime
                font.pixelSize: 20
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "剩余座位: " + flightInfoCard.remainingSeats
                font.pixelSize: 20
                color: flightInfoCard.remainingSeats > 0 ? "#27AE60" : "#C0392B"
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignRight

                Button {
                    text: "预订"
                    font.pixelSize: 18
                    background: Rectangle {
                        color: "#3498DB"
                        radius: 10
                    }
                    padding: 12
                    onClicked: {
                        // 处理支付逻辑
                        console.log("预订航班: " + flightInfoCard.flightNumber)
                    }
                }

                Button {
                    text: "收藏"
                    font.pixelSize: 18
                    background: Rectangle {
                        color: "#F1C40F"
                        radius: 10
                    }
                    padding: 12
                    onClicked: {
                        // 处理收藏逻辑
                        console.log("收藏航班: " + flightInfoCard.flightNumber)
                    }
                }
            }
        }
    }
}
