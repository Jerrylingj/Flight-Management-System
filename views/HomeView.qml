import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// HomeView.qml
Page {
    id: homeView
    title: qsTr("主页")
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: qsTr("主页")
            font.pointSize: 16
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
