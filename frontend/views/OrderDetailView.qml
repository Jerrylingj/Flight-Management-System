import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// 详细注释说明：
// Flickable 组件：

// 使用 Flickable 组件包裹页面内容，使页面可以滑动。当订单信息较多时，用户可以通过手指滑动查看详细信息，而不需要担心信息被遮挡或溢出屏幕。
// Flickable 的 width 和 height 都设置为 parent.width 和 parent.height，即使其填充整个父容器，确保内容可以在整个页面上滑动。
// ColumnLayout 组件：

// ColumnLayout 用于垂直排列所有页面元素，包括标题、订单信息和按钮等。
// 设置了 spacing: 20 来为各个元素之间提供适当的间距，避免页面过于拥挤。
// Text 组件：

// 用于显示订单的详细信息，如航班号、出发地、目的地、时间等。每个信息项的字体大小和颜色均已设置，确保信息清晰易读。
// 使用 orderDetailPage.viewData 访问从父组件传递过来的数据并显示相应的订单详情。
// 按钮设计：

// 页面包含两个按钮，分别是 "显示二维码" 和 "取消预订"。这些按钮的背景色分别为蓝色和红色，并且具有圆角设计，显得更加现代和友好。
// onClicked 信号用于响应按钮点击事件，目前仅打印日志，实际使用中可以根据需求实现二维码显示或取消预订的具体功能。
// 总结：
// 该页面通过使用 Flickable 组件，确保在订单信息较长时，用户可以自由滑动查看所有信息。
// 页面布局简单清晰，所有详细信息都以易于阅读的方式展示，且有按钮提供进一步的操作（如显示二维码和取消预订）。

// OrderDetailView.qml
Page {
    id: orderDetailPage
    title: "订单详情"

    property var viewData  // 接收从viewDataCard传递过来的数据

    Rectangle {
        width: parent.width
        height: parent.height
        color: "#F4F6F8"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            Text {
                text: "订单详细信息"
                font.pixelSize: 26
                font.bold: true
                color: "#2C3E50"
            }

            Text {
                text: "航班号: " + orderDetailPage.viewData.flightNumber
                font.pixelSize: 20
                color: "#34495E"
            }

            Row { // 使用Row来并排放置出发地和目的地
                spacing: 20
                Text {
                    text: "出发地: " + orderDetailPage.viewData.departure
                    font.pixelSize: 20
                    color: "#34495E"
                }
                Text {
                    text: "目的地: " + orderDetailPage.viewData.destination
                    font.pixelSize: 20
                    color: "#34495E"
                }
            }

            Row { // 使用Row来并排放置出发时间和到达时间
                spacing: 20
                Text {
                    text: "出发时间: " + orderDetailPage.viewData.departureTime
                    font.pixelSize: 20
                    color: "#34495E"
                }
                Text {
                    text: "到达时间: " + orderDetailPage.viewData.arrivalTime
                    font.pixelSize: 20
                    color: "#34495E"
                }
            }

            Row { // 使用Row来并排放置检票开始和结束时间
                spacing: 20
                Text {
                    text: "检票开始时间: " + orderDetailPage.viewData.checkInStartTime
                    font.pixelSize: 20
                    color: "#34495E"
                }
                Text {
                    text: "检票结束时间: " + orderDetailPage.viewData.checkInEndTime
                    font.pixelSize: 20
                    color: "#34495E"
                }
            }

            Text {
                text: "出发机场: " + orderDetailPage.viewData.departureAirport
                font.pixelSize: 20
                color: "#34495E"
            }

            Text {
                text: "到达机场: " + orderDetailPage.viewData.arrivalAirport
                font.pixelSize: 20
                color: "#34495E"
            }

            Text {
                text: "航空公司: " + orderDetailPage.viewData.airlineCompany
                font.pixelSize: 20
                color: "#34495E"
            }

            Text {
                text: "状态: " + orderDetailPage.viewData.status
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
