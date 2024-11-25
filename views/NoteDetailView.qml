import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Effects

// OrdersView.qml
Page {
    id: noteDetailView

    property string viewName: '笔记详情'
    property var viewData

    Flickable{
        id: flickableContainer
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: column.height
        clip:true
        Column {
            id: column
            height: userInfo.height + articleContent.height + imageContainer.height
            width: flickableContainer.contentWidth
            Row { // 头像跟名字
                id: userInfo
                spacing: 20
                width:parent.width
                height: Math.max(avatarRect.height,userNameRect.height)

                Rectangle{
                    id: avatarRect
                    width: 50
                    height: 50
                    radius: width/2
                    color:"transparent"
                    Image {
                        // 圆框框头像，ai给的看不懂
                        id: avatar
                        anchors.fill: parent
                        source: viewData.author.coverImage.dynamicUrl
                        fillMode: Image.PreserveAspectCrop
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            maskEnabled: true
                            maskSource: ShaderEffectSource {
                                sourceItem: Rectangle {
                                    width: avatar.width
                                    height: avatar.height
                                    radius: width/2
                                    color: "black"
                                }
                            }
                        }
                    }
                }

                Rectangle{
                    id: userNameRect
                    width: userName.width
                    height: Math.max(userName.implicitHeight,avatarRect.height) //统一高度
                    color:"transparent"
                    z:-1
                    Text {
                        id: userName
                        anchors.verticalCenter: parent.verticalCenter // 竖向居中
                        text: qsTr(viewData.author.nickName)
                        wrapMode: Text.WordWrap
                    }
                }
                Rectangle {
                    id: articleTitle
                    color: "transparent"
                    width: title.width
                    height: Math.max(userName.implicitHeight,avatarRect.height)
                    Text {
                        id: title
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr(viewData.articleTitle)
                        font.bold: true
                        font.pixelSize: 16
                    }
                }
            }
            Rectangle {
                id: articleContent
                y:userInfo.height
                width: parent.width
                height:articleText.height
                Text {
                    id: articleText
                    text: qsTr(viewData.content)
                    wrapMode: Text.Wrap
                    width: parent.width
                    font.pixelSize: 16
                }
            }

            Repeater {
                id: imageContainer
                height: 0
                model: viewData.images
                delegate: Rectangle {
                    height: 200
                    width: parent.width
                    Image {
                        id: articleImage
                        height: parent.height
                        width: parent.width
                        source: modelData.dynamicUrl
                    }
                    Component.onCompleted: {
                        imageContainer.height += height
                    }
                }
            }

        }
    }
}
