import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    visible: true
    property StackView stack: StackView.view
    // 登录框
    Rectangle {
        anchors.left: parent.left // 左边对齐到父元素的左边
        anchors.leftMargin: parent.width * 0.05
        width: parent.width * 0.9
        height: 480
        anchors.centerIn: parent
        radius: 15
        color: "white"
        border.color: "#ddd"
        border.width: 1
        // 阴影效果
        smooth: true

        // 登录表单
        Column {
            anchors.fill: parent
            spacing: 25

            // 标题
            Text {
                // anchors.top:parent.top
                // anchors.topMargin: 20
                text: "编辑个人信息"
                font.bold: true
                font.pixelSize: 28
                color: "#00796b"
                anchors.horizontalCenter: parent.horizontalCenter  // 确保水平居中
            }
            Image {
                source: "../figure/editinfo.png"
                width: 100
                anchors.horizontalCenter: parent.horizontalCenter  // 确保水平居中
                height: width
            }

            // 用户名
            TextField {
                id: userNameText
                placeholderText: "用户名"
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                font.pixelSize: 18
                height: 40
                padding: 10
                color: "#00796b"
            }

            // 个人信息
            TextField {
                id:userPersonalInfoText
                placeholderText: "个人信息"
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                font.pixelSize: 18
                height: 80
                padding: 10
                color: "#00796b"
            }

            // 确定按钮
            Button {
                text: "确定"
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                height: 40
                font.pixelSize: 18
                background: Rectangle {
                    color: "#00bcd4"
                    radius: 8
                }
                onClicked: {
                    if(userNameText.text.length>5){
                        console.log("名字过长")
                    }
                    else{
                        userInfo.userName=userNameText.text
                        userInfo.userPersonalInfo=userPersonalInfoText.text
                        stack.changeTo("views/ProfileView.qml")
                        console.log("确定更改")
                    }
                }
            }
        }
    }
}
