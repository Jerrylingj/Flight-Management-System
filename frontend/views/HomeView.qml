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
        property var selectedCity
        onSelectedCityChanged: {
            stack.changeTo('views/NoteDetailView.qml',selectedCity)
        }
    }
}
