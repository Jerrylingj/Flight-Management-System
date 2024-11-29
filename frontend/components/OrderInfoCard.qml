import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// OrderInfoCard.qml
Item {
    id: orderInfoCard
    width: parent.width
    height: 220

    property string flightNumber
    property string departure
    property string destination
    property string departureTime
    property string arrivalTime
    property string checkInTime

    property StackView stack  // 确保 stack 被正确传递

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
                    text: "航班号: " + orderInfoCard.flightNumber
                    font.pixelSize: 24
                    font.bold: true
                    color: "#2C3E50"
                    Layout.alignment: Qt.AlignLeft
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                spacing: 20

                Text {
                    text: "出发地: " + orderInfoCard.departure
                    font.pixelSize: 20
                    color: "#34495E"
                    Layout.alignment: Qt.AlignLeft
                }

                Text {
                    text: "目的地: " + orderInfoCard.destination
                    font.pixelSize: 20
                    color: "#34495E"
                    Layout.alignment: Qt.AlignLeft
                }
            }

            Text {
                text: "出发时间: " + orderInfoCard.departureTime + " - 到达时间: " + orderInfoCard.arrivalTime
                font.pixelSize: 20
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                text: "检票时间: " + orderInfoCard.checkInTime
                font.pixelSize: 20
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignRight

                Button {
                    text: "查看"
                    font.pixelSize: 18
                    background: Rectangle {
                        color: "#2BC43A"  // 嫩绿色
                        radius: 10
                    }
                    padding: 12
                    onClicked: {
                        // 确保 stack 已正确传递并且有效
                        if (stack) {
                            stack.changeTo("views/OrderDetailView.qml", {
                                flightNumber: orderInfoCard.flightNumber,
                                departure: orderInfoCard.departure,
                                destination: orderInfoCard.destination,
                                departureTime: orderInfoCard.departureTime,
                                arrivalTime: orderInfoCard.arrivalTime,
                                checkInTime: orderInfoCard.checkInTime
                            })
                        } else {
                            console.log("StackView is not available.")
                        }
                    }
                }
            }
        }
    }
}
