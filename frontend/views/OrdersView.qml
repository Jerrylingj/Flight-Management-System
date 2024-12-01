import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../components/"

// OrdersView.qml
Page {
    id: ordersView

    // 页面名称属性
    property string viewName: '全部订单'

    // 用于页面切换的 StackView
    property StackView stack: StackView.view

    // 使用 Flickable 来替代 ScrollView，允许用户滚动浏览内容
    Flickable {
        width: parent.width
        height: parent.height
        contentWidth: parent.width
        contentHeight: ordersList.height  // 让内容的高度为 ordersList 的高度

        // 设置是否允许垂直和水平方向滚动
        flickableDirection: Flickable.VerticalFlick

        // 为订单列表创建一个 ColumnLayout，用来垂直排列订单信息
        ColumnLayout {
            id: ordersList
            spacing: 20
            anchors.margins: 20
            width: parent.width

            // 用于传递和更新订单详细信息的属性
            property var info

            // 当 info 发生变化时，切换到订单详情页面
            onInfoChanged: {
                stack.changeTo("views/OrderDetailView.qml", info)
            }

            // Repeater 用于创建多个订单信息卡片
            Repeater {
                model: 5  // 假设展示5条订单信息，实际项目中可以绑定动态数据模型
                delegate: OrderInfoCard {
                    width: parent.width
                    height: 220
                    flightNumber:"CA123" + index
                    departure:"北京"
                    destination:"上海"
                    departureTime:"08:00"
                    arrivalTime:"10:00"
                    checkInTime:"07:00"
                }
            }
        }
    }
}
