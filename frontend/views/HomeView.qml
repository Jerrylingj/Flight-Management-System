import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// HomeView.qml
Page {
    id: homeView
    property string viewName: '主页'

    property StackView stack: StackView.view

    Loader {
        source: "../components/NoteList.qml"
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        // anchors.topMargin: 200 + 16 // 设置与图片的间隔
        property var selectedCity
        onSelectedCityChanged: {
            stack.changeTo("views/NoteDetailView.qml", selectedCity)
        }
    }


}
