import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import NetworkHandler 1.0

Page {
    visible: true
    property string userNmae: "none"
    property string passWord: "none"
    NetworkHandler{

    }

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
                text: "注册界面"
                font.bold: true
                font.pixelSize: 28
                color: "#00796b"
                anchors.horizontalCenter: parent.horizontalCenter  // 确保水平居中
            }
            Image {
                source: "../figure/login-flight.png"
                width: 100
                anchors.horizontalCenter: parent.horizontalCenter  // 确保水平居中
                height: width
            }

            // 用户名输入框
            TextField {
                placeholderText: "Username"
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                font.pixelSize: 18
                height: 40
                padding: 10
                color: "#00796b"
            }

            // 密码输入框
            TextField {
                placeholderText: "Password"
                id: pssField

                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                font.pixelSize: 18
                height: 40
                padding: 10
                echoMode: TextInput.Password
                color: "#00796b"
            }

            // 密码输入框
            TextField {
                placeholderText: "Confirm password"
                id: pssConirmField
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                font.pixelSize: 18
                height: 40
                padding: 10
                echoMode: TextInput.Password
                color: "#00796b"
            }

            // 登录按钮
            Button {
                text: "注册"
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
                    // 注册逻辑
                    if(pssField.text!=pssConirmField.text){
                        console.log("两次密码输入不同")
                        confirmationDialog.open()

                    }
                    else{
                        passWord=pssField.text
                        console.log("注册")
                    }
                }
                Dialog {
                    id: confirmationDialog
                    standardButtons: Dialog.Yes
                    x: (parent.width - width) / 2
                    y: (parent.height - height) / 2
                    parent: Overlay.overlay

                    modal: true
                    Column {
                        spacing: 20
                        anchors.fill: parent
                        Label {
                            text: qsTr("两次密码输入不同！")
                        }
                        CheckBox {
                            text: qsTr("Do not ask again")
                            anchors.right: parent.right
                        }
                    }
                }
            }

        }
    }
}
