import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    width: 425
    height: 772
    visible: true
    title: qsTr(pageName.nameText+"|航班信息管理系统")

    Rectangle{
        id:pageNameRect
        anchors.top: parent.top
        height:pageName.height
        width: parent.width
        color:"white"
        z:3
        Text {
            id:pageName
            property string nameText: "主页"
            text: qsTr(nameText)
            font.pointSize: 16
            anchors.centerIn: parent
            color:"black"
        }
    }


    // 管理页面
    StackView {
        id: rootStackView
        anchors.top: pageNameRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomNavBar.top
        height: parent.height - pageNameRect.height - bottomNavBar.height
        initialItem: "views/HomeView.qml"
        property alias view: rootStackView
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
                    pageName.nameText = "主页"
                    rootStackView.push(Qt.resolvedUrl("views/HomeView.qml"))
                }
            }
            Button {
                text: qsTr("航班信息")
                Layout.fillWidth: true
                // icon.source: "images/flight_icon.png"
                onClicked: {
                    // 航班信息
                    console.log("航班信息")
                    pageName.nameText = "航班信息"
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
                    pageName.nameText = "全部订单"
                    rootStackView.push(Qt.resolvedUrl("views/OrdersView.qml"))
                }
            }
            Button {
                text: qsTr("个人中心")
                Layout.fillWidth: true
                // icon.source: "images/profile_icon.png"
                onClicked: {
                    pageName.nameText = "个人中心"
                    rootStackView.push(Qt.resolvedUrl("views/ProfileView.qml"))
                }
            }
        }
    }

}
