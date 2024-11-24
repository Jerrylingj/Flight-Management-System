import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ProfileView.qml
Page {
    id: profileView
    Rectangle {
        anchors.fill: parent
        color: "#F5F5F5" // 背景颜色

        Column {
            anchors.fill: parent
            anchors.centerIn: parent
            spacing: 20

            // 头像和用户名部分
            Row {
                spacing: 10
                anchors.horizontalCenter: parent
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
                    text: "用户名"
                    font.bold: true
                    font.pointSize: 20
                    color: "#333333"
                }

            }

            // 个人信息文本框
            Text {
                text: "你好，我是用户名，我来自xxx，欢迎大家和我交流。"
                font.pointSize: 14
                color: "#666666"
                wrapMode: Text.Wrap // 自动换行
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent
                width: parent.width * 0.95 // 增加宽度到95%
            }

            // 按钮部分
            Rectangle {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05 // 左边距增加 20 像素
                width: parent.width * 0.9
                height: 50
                color: "#FFFFFF"
                radius: 8
                border.color: "#CCCCCC"

                Text {
                    anchors.centerIn: parent
                    text: "我的订单"
                    font.pointSize: 16
                    color: "#333333"
                }
            }

            Rectangle {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05 // 左边距增加 20 像素
                width: parent.width * 0.9
                height: 50
                color: "#FFFFFF"
                radius: 8
                border.color: "#CCCCCC"

                Text {
                    anchors.centerIn: parent
                    text: "登录账号"
                    font.pointSize: 16
                    color: "#333333"
                }
            }

            Rectangle {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05 // 左边距增加 20 像素
                width: parent.width * 0.9
                height: 50
                color: "#FFFFFF"
                radius: 8
                border.color: "#CCCCCC"

                Text {
                    anchors.centerIn: parent
                    text: "注销账号"
                    font.pointSize: 16
                    color: "#333333"
                }
            }

            Rectangle {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05 // 左边距增加 20 像素
                width: parent.width * 0.9
                height: 50
                color: "#FFFFFF"
                radius: 8
                border.color: "#CCCCCC"

                Text {
                    anchors.centerIn: parent
                    text: "退出登录"
                    font.pointSize: 16
                    color: "#333333"
                }
            }
        }
    }
}
