import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// OrdersView.qml
Page {
    id: ordersView

    // 页面名称属性
    property string viewName: '全部订单'

    // 用于页面切换的 StackView
    property StackView stack: StackView.view

    // 用于传递和更新订单详细信息的属性
    property var info

    // 当 info 发生变化时，切换到订单详情页面
    onInfoChanged: {
        stack.changeTo("views/OrderDetailView.qml", info)
    }

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

            // Repeater 用于创建多个订单信息卡片
            Repeater {
                model: 5  // 假设展示5条订单信息，实际项目中可以绑定动态数据模型
                Loader {
                    source: "../components/OrderInfoCard.qml"  // 载入每个订单信息卡片的模板
                    property var orderInfo: {
                        "flightNumber": "CA123" + index,  // 示例航班号，基于索引生成
                        "departureTime": "08:00",  // 示例起飞时间
                        "arrivalTime": "10:00",  // 示例到达时间
                        "departureCity": "北京",  // 示例出发城市
                        "arrivalCity": "上海",  // 示例到达城市
                        "checkInTime": "07:00"  // 示例登机时间
                    }

                    // 传递当前订单的 selectedInfo 数据，供卡片使用
                    property var selectedInfo: info

                    // 当 Loader 加载完成后，填充数据到卡片组件
                    onLoaded: {
                        item.flightNumber = orderInfo.flightNumber;  // 设置航班号
                        item.departure = orderInfo.departureCity;  // 设置出发城市
                        item.destination = orderInfo.arrivalCity;  // 设置到达城市
                        item.departureTime = orderInfo.departureTime;  // 设置起飞时间
                        item.arrivalTime = orderInfo.arrivalTime;  // 设置到达时间
                        item.checkInTime = orderInfo.checkInTime;  // 设置登机时间
                    }
                }
            }
        }
    }
}
