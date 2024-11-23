import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    width: 300
    height: 772
    visible: true
    title: qsTr("Flight Management System")


    // 管理页面
    StackView {
        id: rootStackView
        anchors.top: navbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomNavBar.top
        initialItem: "views/HomeView.qml"
    }

    NoteList {
        visible: true
        width:parent.width
        height:parent.height
    }

    // 底部导航栏
    Rectangle {
        id: bottomNavBar
        width: parent.width
        height: 60
        color: "#3E4E5E"
        anchors.bottom: parent.bottom

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Button {
                text: qsTr("主页")
                Layout.fillWidth: true
                // icon.source: "images/home_icon.png"
                onClicked: {
                    // 主页
                    console.log("主页")
                    rootStackView.push(Qt.resolvedUrl("HomeView.qml"))
                }
            }
            Button {
                text: qsTr("航班信息")
                Layout.fillWidth: true
                // icon.source: "images/flight_icon.png"
                onClicked: {
                    // 航班信息
                    console.log("航班信息")
                    rootStackView.push(Qt.resolvedUrl("views/FlightInfoView.qml"))
                }
            }
            Button {
                text: qsTr("全部订单")
                Layout.fillWidth: true
                // icon.source: "images/orders_icon.png"
                onClicked: {
                    // 全部订单
                    console.log("全部订单")
                    rootStackView.push(Qt.resolvedUrl("views/OrdersView.qml"))
                }
            }
            Button {
                text: qsTr("个人中心")
                Layout.fillWidth: true
                // icon.source: "images/profile_icon.png"
                onClicked: {
                    rootStackView.push(Qt.resolvedUrl("views/ProfileView.qml"))
                }
            }
        }
    }

}
