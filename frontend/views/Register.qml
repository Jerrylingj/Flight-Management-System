import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import NetworkHandler 1.0

Page {
    property string viewName: "注册"
    property StackView stack: StackView.view
    visible: true
    property string myValue: ""
    NetworkHandler{
        id: networkHandler

        onRequestSuccess: function(responseData) {
            if(responseData["code"]!==200){
                console.log(responseData["message"])
                return
            }
            if(responseData["data"].length> 10){
                myValue = responseData["data"];
                console.log(myValue);
            }else{
                userInfo.userName=usernameField.text
                userInfo.userPersonalInfo="Some personal info about "+userInfo.userName+"."
                stack.changeTo('views/ProfileView.qml')
            }
        }

        onRequestFailed: function(errorMessage) {
            console.log("请求失败：", errorMessage); // 打印失败的错误信息

        }
    }

    function useDiolog(msg){
        failDiolog.msg = msg
        failDiolog.open()
    }

    Dialog {
        id: failDiolog
        standardButtons: Dialog.Yes
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay
        property string msg

        modal: true
        Column {
            spacing: 20
            anchors.fill: parent
            Label {
                text: qsTr(failDiolog.msg)
            }
            CheckBox {
                text: qsTr("Do not ask again")
                anchors.right: parent.right
            }
        }
    }

    // 登录框
    Rectangle {
        anchors.left: parent.left // 左边对齐到父元素的左边
        anchors.leftMargin: parent.width * 0.05
        width: parent.width * 0.9
        height: 580
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
                id:usernameField
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
                placeholderText: "Email"
                id:emailField
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

            TextField {
                placeholderText: "code"
                id: codeField
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.1
                width: parent.width * 0.8
                font.pixelSize: 18
                height: 40
                padding: 10
                color: "#00796b"
            }

            Button {
                text: "发送验证码"
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
                    if(emailField.text.length === 0){
                        useDiolog("请输入邮箱")
                    }
                    else{
                        networkHandler.request("/api/send-code",NetworkHandler.POST,{
                                                   "email":emailField.text
                                               })
                        console.log("发送验证码")
                    }
                }
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
                        useDiolog("两次密码不一致")
                    }
                    else{
                        networkHandler.request("/api/register",NetworkHandler.POST,{
                                                   "username":userInfo.userName,
                                                   "password":pssField.text,
                                                   "email":emailField.text,
                                                   "code":codeField.text,
                                                   "value":myValue
                                               })
                    }
                }
            }

        }
    }
}
