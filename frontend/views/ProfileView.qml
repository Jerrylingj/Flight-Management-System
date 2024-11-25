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

            }

            // 剩余金额
            Text {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                text: "剩余金额：￥"+myMoney
                font.pointSize: 20
                color: "#EE82EE"
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

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
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
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
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
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
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


            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
                width: parent.width * 0.8
                height: 40
                text: "编辑个人信息"
                font.pixelSize: 20
                background: Rectangle {
                    color: "#3498DB"
                    radius: 10
                }
                padding: 12
                onClicked: {
                    console.log("编辑个人信息")
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
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
                }
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
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
                }
            }
        }
    }
}
