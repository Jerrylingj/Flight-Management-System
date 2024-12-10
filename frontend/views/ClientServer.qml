import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Page{
    property string viewName: '人工客服'
    property string clientType: "people"
    visible: true

    Flickable {
        anchors.top: parent.top
        anchors.topMargin: 20
        width: parent.width * 0.9
        height: parent.height
        contentHeight:  contenColumn.implicitHeight
        anchors.horizontalCenter: parent.horizontalCenter
        flickableDirection: Flickable.VerticalFlick

        Column {
            // 人工客服按钮
            id: contenColumn
            width: parent.width
            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.15
                width: parent.width * 0.7
                height: 40
                text: "人工客服"
                font.pixelSize: 20

                background: Rectangle {
                    color: "#4CAF50"
                    radius: height * 0.02
                }
                onClicked: {
                    // 打开人工客服页面逻辑
                    viewName="人工客服"
                    clientType="people"
                    console.log("进入人工客服")
                }
            }

            // AI客服按钮
            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.15
                width: parent.width * 0.7
                height: 40
                font.pixelSize: 20
                text: "切换到AI客服"

                background: Rectangle {
                    color: "#FF5722"
                    radius: height * 0.02
                }
                onClicked: {
                    // 打开AI客服页面逻辑
                    viewName="AI客服"
                    clientType="ai"
                    console.log("进入AI客服")
                }
            }
            spacing: 50
            anchors.fill: parent
            // 示例聊天记录
            TextArea {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.15
                width: parent.width * 0.7
                text: "您好，有什么可以帮助您的吗？"
                font.pixelSize: 15
                color: "#757575"
            }

            TextArea {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.15
                width: parent.width * 0.7
                Layout.fillWidth: true
                placeholderText: "请输入您的问题"
                font.pixelSize: 15
            }

            Button {
                anchors.left: parent.left // 左边对齐到父元素的左边
                anchors.leftMargin: parent.width * 0.15
                width: parent.width * 0.7
                height: 40
                font.pixelSize: 20
                text: "发送"
                background: Rectangle {
                    color: "#FF5722"
                    radius: height * 0.02
                }
                onClicked: {
                    // 发送消息逻辑
                    console.log("发送消息")
                }
            }
        }
    }

}
