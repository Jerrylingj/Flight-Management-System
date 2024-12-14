import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import NetworkHandler 1.0

Page {
    visible: true
    property StackView stack: StackView.view
    NetworkHandler{
        id: networkHandler

        onRequestSuccess: function(responseData) {
            if(responseData["code"]!==200&&responseData["code"]!==500){
                console.log("网络问题")
            }
            if(responseData["code"]===500){
                console.log("用户不存在")
                confirmationDialog.open()
                return
            }

            userInfo.userName=userNameField.text
            userInfo.userPersonalInfo="Some personal info about "+userInfo.userName+"."
            console.log("登录成功")

            userInfo.myToken=responseData["data"]["token"]
            stack.changeTo('views/ProfileView.qml')
        }

        onRequestFailed: function(errorMessage) {
            console.log("请求失败：", errorMessage); // 打印失败的错误信息
        }
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
                text: "登录界面"
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
                id:userNameField
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                font.pixelSize: 18
                height: 40
                padding: 10
                color: "#00796b"
            }

            // 电话号码输入框
            TextField {
                placeholderText: "Telephone"
                id:telephoneField
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
                id:userPssField
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
                text: "登录"
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
                    networkHandler.request("/api/login",NetworkHandler.POST,{
                                               "password":userPssField.text,
                                               "telephone":telephoneField.text
                                           })
                    console.log("Login clicked")
                }
            }

            Dialog {
                id: confirmationDialog
                visible: false
                standardButtons: Dialog.Yes
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                parent: Overlay.overlay
                modal: true
                Label {
                    text: qsTr("用户不存在！")
                }

            }

            // 忘记密码链接
            Text {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.05
                text: "Forgot Password?"
                font.pixelSize: 18
                color: "#00796b"
                horizontalAlignment: Text.AlignHCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Forgot Password clicked")
                    }
                }
            }
        }
    }
}
