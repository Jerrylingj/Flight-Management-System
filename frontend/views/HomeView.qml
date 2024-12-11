import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// HomeView.qml
Page {
    id: homeView
    property string viewName: '主页'

    property StackView stack: StackView.view

    Image {
        source: "../figure/homepage-cover.png"
        width: parent.width
        height: 200
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Loader {
        source: "../components/NoteList.qml"
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 200 + 16 // 设置与图片的间隔
        property var selectedCity
        onSelectedCityChanged: {
            stack.changeTo("views/NoteDetailView.qml", selectedCity)
        }
    }


}
