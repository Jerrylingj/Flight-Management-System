import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// OrdersView.qml
Page {
    id: ordersView
    title: qsTr("全部订单")
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: qsTr("全部订单")
            font.pointSize: 16
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
