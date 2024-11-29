import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// OrderDetailView.qml
Page {
    id: orderDetailPage
    title: "订单详情"

    property var orderInfo  // 接收从OrderInfoCard传递过来的数据

    Rectangle {
        width: parent.width
        height: parent.height
        color: "#F4F6F8"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20

            // 显示航班的详细信息
            Text {
                text: "订单详细信息"
                font.pixelSize: 26
                font.bold: true
                color: "#2C3E50"
            }

            Text {
                text: "航班号: " + orderDetailPage.orderInfo.flightNumber
                font.pixelSize: 20
                color: "#34495E"
            }

            Text {
                text: "出发地: " + orderDetailPage.orderInfo.departure
                font.pixelSize: 20
                color: "#34495E"
            }

            Text {
                text: "目的地: " + orderDetailPage.orderInfo.destination
                font.pixelSize: 20
                color: "#34495E"
            }

            Text {
                text: "出发时间: " + orderDetailPage.orderInfo.departureTime + " - 到达时间: " + orderDetailPage.orderInfo.arrivalTime
                font.pixelSize: 20
                color: "#34495E"
            }

            Text {
                text: "检票时间: " + orderDetailPage.orderInfo.checkInTime
                font.pixelSize: 20
                color: "#34495E"
            }

            // 按钮
            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignCenter

                Button {
                    text: "显示二维码"
                    font.pixelSize: 18
                    background: Rectangle {
                        color: "#188DF1"  // 蓝色
                        radius: 10
                    }
                    padding: 12
                    onClicked: {
                        console.log("显示二维码")
                    }
                }

                Button {
                    text: "取消预订"
                    font.pixelSize: 18
                    background: Rectangle {
                        color: "#F10000"  // 红色
                        radius: 10
                    }
                    padding: 12
                    onClicked: {
                        console.log("取消预订")
                    }
                }
            }
        }
    }
}
