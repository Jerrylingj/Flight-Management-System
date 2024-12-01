import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// OrderInfoCard.qml
Item {
    id: orderInfoCard

    // 定义属性，用于接收父组件传递过来的订单信息
    property string flightNumber
    property string departure
    property string destination
    property string departureTime
    property string arrivalTime
    property string checkInTime

    Rectangle {
        width: parent.width - 40
        height: parent.height
        radius: 20
        color: "#FAFAFA"  // 背景颜色
        border.color: "green"  // 设置边框颜色为绿色
        border.width: 2  // 设置边框宽度
        anchors.horizontalCenter: parent.horizontalCenter  // 居中显示

        Column {
            anchors.fill: parent  // 填充父容器
            anchors.margins: 20  // 边距
            spacing: 12  // 垂直间距

            // 第一行显示航班号
            RowLayout {
                Layout.fillWidth: true  // 填充宽度
                Layout.alignment: Qt.AlignLeft  // 左对齐
                spacing: 20  // 行内间距

                Text {
                    text: "航班号: " + orderInfoCard.flightNumber
                    font.pixelSize: 24
                    font.bold: true
                    color: "#2C3E50"  // 字体颜色
                    Layout.alignment: Qt.AlignLeft  // 左对齐
                }
            }

            // 第二行显示出发地和目的地
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

            // 显示出发时间和到达时间
            Text {
                text: "出发时间: " + orderInfoCard.departureTime + " - 到达时间: " + orderInfoCard.arrivalTime
                font.pixelSize: 20
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            // 显示检票时间
            Text {
                text: "检票时间: " + orderInfoCard.checkInTime
                font.pixelSize: 20
                color: "#34495E"
                Layout.alignment: Qt.AlignLeft
            }

            // 最后一行显示 "查看" 按钮
            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignRight  // 右对齐

                Button {
                    text: "查看"
                    font.pixelSize: 18
                    background: Rectangle {
                        color: "#2BC43A"  // 按钮背景色
                        radius: 10  // 圆角
                    }
                    padding: 12  // 按钮内边距

                    // 按钮点击时，传递信息到上级组件
                    onClicked: {
                        // 当点击查看按钮时，将订单信息传递给上级组件
                        orderInfoCard.parent.info = {
                            flightNumber: orderInfoCard.flightNumber,
                            departure: orderInfoCard.departure,
                            destination: orderInfoCard.destination,
                            departureTime: orderInfoCard.departureTime,
                            arrivalTime: orderInfoCard.arrivalTime,
                            checkInTime: orderInfoCard.checkInTime
                        }
                    }
                }
            }
        }
    }
}
