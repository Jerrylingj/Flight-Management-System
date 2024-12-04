import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ProfileView.qml
Page {
    id: profileView

    property string viewName: '个人中心'
    property string userName: '未知用户'
    property string personalInfo: "你好，我是未知用户，我来自xxx，欢迎大家和我交流。"
    property int myMoney: 114514
    property StackView stack: StackView.view

    Rectangle {
        anchors.fill: parent
        color: "#F5F5F5" // 背景颜色

        Column {
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.fill: parent
            anchors.centerIn: parent
            spacing: 20


            // 头像和用户名部分
            Row {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
                spacing: 10
                Rectangle {
                    width: 80
                    height: 80
                    radius: 40
                    color: "#FFD700" // 圆形背景颜色
                    Text {
                        anchors.centerIn: parent
                        text: "头像"
                        color: "white"
                        font.bold: true
                        font.pointSize: 12
                    }
                }

                Text {
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    text: userName
                    font.bold: true
                    font.pointSize: 20
                    color: "#333333"
                }

                Button {
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    width: parent.width * 0.4
                    height: 40
                    text: "编辑"
                    font.pixelSize: 20
                    background: Rectangle {
                        color: "#3498DB"
                        radius: 10
                    }
                    padding: 12
                    onClicked: {
                        console.log("编辑个人信息")
                        stack.changeTo("views/EditPersonalInfo.qml")
                    }
                }

            }

            // 个人信息文本框
            Text {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
                text: "个人简介"
                font.pointSize: 15
                color: "#008000"
            }

            Text {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
                text: "    " + personalInfo
                font.pointSize: 14
                color: "#666666"
                wrapMode: Text.Wrap // 自动换行
                horizontalAlignment: Text.AlignLeft // 修改这里为左对齐
                width: parent.width * 0.9 // 增加宽度到90%
            }

            // 剩余金额
            Text {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                text: "剩余金额：￥"+myMoney
                font.pointSize: 20
                color: "#EE82EE"
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                height: 40
                text: "我的订单"
                font.pixelSize: 20
                background: Rectangle {
                    color: "#3498DB"
                    radius: 10
                }
                padding: 12
                onClicked: {
                    console.log("我的订单")
                    stack.changeTo("views/OrdersView.qml")
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                height: 40
                text: "我的收藏"
                font.pixelSize: 20
                background: Rectangle {
                    color: "#3498DB"
                    radius: 10
                }
                padding: 12
                onClicked: {
                    console.log("我的收藏")
                    stack.changeTo("views/FlightInfoView.qml")
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                height: 40
                text: "登录/切换账号"
                font.pixelSize: 20
                background: Rectangle {
                    color: "#3498DB"
                    radius: 10
                }
                padding: 12
                onClicked: {
                    console.log("登录/切换账号")
                    stack.changeTo('views/login.qml')
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                height: 40
                text: "注销账号"
                font.pixelSize: 20
                background: Rectangle {
                    color: "#3498DB"
                    radius: 10
                }
                padding: 12
                onClicked: {
                    console.log("注销账号")
                    erase_dialog.open()
                }
            }
            Dialog{
                id:erase_dialog
                visible:false
                modal:true
                width: parent.width * 0.6
                x: (parent.width - width) / 2  // 居中水平位置
                y: 10                    // 顶部位置
                contentItem: Rectangle {
                    color: "#f0f0f0"
                    implicitHeight: 100
                    Column{
                        anchors.fill: parent
                        anchors.centerIn: parent
                        spacing: 60
                        Label {
                            anchors.horizontalCenter: parent.horizontalCenter  // 确保水平居中
                            text: "确定注销账号"
                            font.pointSize: 20
                            color: "#8B0000"
                        }
                        Row{
                            anchors.centerIn: parent
                            spacing: 30
                            Button{
                                text: "确定"
                                font.pointSize: 14
                                onClicked: {
                                    erase_dialog.close()
                                    console.log("确定")
                                }
                                background: Rectangle {
                                    color: "#90EE90"
                                    radius: 5
                                }
                                height: 30
                            }
                            Button{
                                text: "取消"
                                font.pointSize: 14
                                onClicked: {
                                    erase_dialog.close()
                                    console.log("取消")
                                }
                                background: Rectangle {
                                    color: "#90EE90"
                                    radius: 5
                                }
                                height: 30
                            }
                        }
                    }
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                height: 40
                text: "注册账号"
                font.pixelSize: 20
                background: Rectangle {
                    color: "#3498DB"
                    radius: 10
                }
                padding: 12
                onClicked: {
                    console.log("注册账号")
                }
            }
        }
    }
}
