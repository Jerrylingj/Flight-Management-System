import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NetworkHandler 1.0
Page{
    property string viewName: '人工客服'
    property string clientType: "people"
    visible: true
    property var messages:[]
    height: parent.height
    width: parent.width

    NetworkHandler{
        id:networkHandler
        onRequestSuccess:function(data) {
            console.log(JSON.stringify(data,null,2))
            const msg = data['data']
            messages.push(msg)
            listModel.append(msg)
        }
        onRequestFailed: function(data){
            console.log(data['message'])
        }
    }

    Column {
        // 人工客服按钮
        id: contenColumn
        height: parent.height
        width: parent.width
        Button {
            id:changeButton
            anchors.leftMargin: parent.width * 0.15
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 0.7
            height: 40
            text: viewName
            font.pixelSize: 20

            background: Rectangle {
                color: "#4CAF50"
                radius: height * 0.02
            }
            onClicked: {
                // 打开人工客服页面逻辑
                viewName = viewName === "人工客服"? "AI客服":"人工客服"
                clientType = viewName === "人工客服"?"people":"AI"
                console.log(`进入${viewName}`)
            }
        }
        Flickable {
            id: flickableContainer
            width: parent.width
            height: parent.height - changeButton.height - textSend.height - sendButton.height
            contentHeight: container.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            flickableDirection: Flickable.VerticalFlick

            Repeater {
                model: ListModel {
                    id: listModel
                }

                id: container
                width: parent.width * 0.8
                height: contentHeight

                property real totalY:0.0

                delegate: Rectangle {
                    width: parent.width *0.8
                    height: text.height + 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"

                    Text {
                        id:text
                        width: parent.width * 0.7
                        height: Text.paintedHeight // Use paintedHeight to adapt text height
                        text: content
                        font.pixelSize: 15
                        color: role === 'user' ? "red" : "blue"
                        wrapMode: Text.Wrap
                        Component.onCompleted: {
                            console.log("text",height)
                        }
                        anchors.left:role === 'user' ?undefined:parent.left
                        anchors.right:role === 'user' ?parent.right:undefined
                    }
                    Component.onCompleted: {
                        y = container.totalY
                        container.totalY += height
                        console.log(x,y,height)
                    }
                }
            }
        }


        TextArea{
            id:textSend
            height: 50
            width:parent.width*0.5
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            id:sendButton
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
                console.log(clientType)
                if(clientType === 'AI'){
                    const msg = {
                        "role":"user",
                        "content":textSend.text
                    }
                    messages.push(msg)
                    listModel.append(msg)
                    textSend.text = ""
                    networkHandler.request("/api/aichat",NetworkHandler.POST,{messages:messages})
                }

            }
        }
    }

}
