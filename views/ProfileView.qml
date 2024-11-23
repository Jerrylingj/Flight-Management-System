import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ProfileView.qml
Page {
    id: profileView
    title: qsTr("个人中心")
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: qsTr("个人中心")
            font.pointSize: 16
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
