import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// HomeView.qml
Page {
    id: homeView

    Loader {
        source: "../components/NoteList.qml"
        width: parent.width
        height: parent.height
    }
}
