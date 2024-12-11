import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    width: 90 * 5
    height: 160 * 5
    visible: true
    title: qsTr(pageName.nameText+"|航班信息管理系统")

    Rectangle{
        id:pageNameRect
        anchors.top: parent.top
        height:pageName.height
        width: parent.width
        color:"white"
        z:3
        Button{
            Text{
                text:qsTr("返回")
                anchors.centerIn: parent
            }

            width: parent.width * 0.1
            height:parent.height
            onClicked: {
                rootStackView.back()
            }
        }

        Text {
            id:pageName
            property string nameText: rootStackView.currentItem.viewName
            text: qsTr(nameText)
            font.pointSize: 20
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
        function changeTo(url,data){
            if(data && Object.keys(data).length !== 0){
                push(Qt.resolvedUrl(url),{viewData:data})
                return
            }
            push(Qt.resolvedUrl(url))
        }
        function back(){
            if(depth>1){
                pop()
            }

        }
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
                    rootStackView.changeTo('views/HomeView.qml')
                }
            }
            Button {
                text: qsTr("航班")
                Layout.fillWidth: true
                // icon.source: "images/flight_icon.png"
                onClicked: {
                    // 航班信息
                    rootStackView.changeTo('views/FlightInfoView.qml')
                }
            }
            Button {
                text: qsTr("客服")
                Layout.fillWidth: true
                // icon.source: "images/orders_icon.png"
                onClicked: {
                    // 全部订单
                    rootStackView.changeTo('views/ClientServer.qml')
                }
            }
            Button {
                text: qsTr("订单")
                Layout.fillWidth: true
                // icon.source: "images/orders_icon.png"
                onClicked: {
                    // 全部订单
                    rootStackView.changeTo('views/OrdersView.qml')
                }
            }
            Button {
                text: qsTr("个人")
                Layout.fillWidth: true
                // icon.source: "images/profile_icon.png"
                onClicked: {
                    rootStackView.changeTo("views/ProfileView.qml")
                }
            }
        }
    }

}
