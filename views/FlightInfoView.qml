import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: flightInfoView

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        // // 一些数据
        // Repeater {
        //     model: ListModel {
        //         ListElement { origin: "Beijing"; destination: "Shanghai"; time: "2024-11-25 08:00"; price: "¥1200" }
        //         ListElement { origin: "Guangzhou"; destination: "Chengdu"; time: "2024-11-25 14:30"; price: "¥1000" }
        //         ListElement { origin: "Shenzhen"; destination: "Hangzhou"; time: "2024-11-25 09:45"; price: "¥900" }
        //         ListElement { origin: "Beijing"; destination: "Chengdu"; time: "2024-11-25 18:00"; price: "¥1300" }
        //     }

        //     delegate: flightInfoCard.createObject(parent, {
        //         "origin": modelData.origin,
        //         "destination": modelData.destination,
        //         "time": modelData.time,
        //         "price": modelData.price
        //     })
        // }

    }

    // // Inline Component Definition
    // Component {
    //     id: flightInfoCard

    //     Rectangle {
    //         width: parent.width
    //         height: 80
    //         color: "#F5F5F5"
    //         radius: 10
    //         border.color: "#E0E0E0"
    //         anchors.margins: 5
    //         Layout.fillWidth: true

    //         RowLayout {
    //             anchors.fill: parent
    //             anchors.margins: 10
    //             spacing: 15

    //             Column {
    //                 Text {
    //                     text: qsTr("From: ") + origin
    //                     font.bold: true
    //                 }
    //                 Text {
    //                     text: qsTr("To: ") + destination
    //                     font.bold: true
    //                 }
    //             }

    //             Column {
    //                 Text {
    //                     text: qsTr("Time: ") + time
    //                 }
    //                 Text {
    //                     text: qsTr("Price: ") + price
    //                     color: "#FF5722" // Highlight the price
    //                     font.bold: true
    //                 }
    //             }
    //         }

    //         property string origin
    //         property string destination
    //         property string time
    //         property string price
    //     }
    // }
}
