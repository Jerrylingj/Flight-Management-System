// TimeProgressBar.qml
import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    width: 400
    height: 100

    // 开始时间和结束时间
    property string startTimeValue
    property string endTimeValue

    // 当前时间
    property string currentTimeValue

    // 解析时间为分钟数的函数
    function parseTime(timeStr) {
        var parts = timeStr.split(":");
        return parseInt(parts[0]) * 60 + parseInt(parts[1]);
    }

    // 解析后的时间
    property int startTime: parseTime(startTimeValue)
    property int endTime: parseTime(endTimeValue)
    property int currentTime: parseTime(currentTimeValue)

    // 计算进度（0 到 1）
    property real progress: {
        if (currentTime <= startTime) return 0;
        if (currentTime >= endTime) return 1;
        return (currentTime - startTime) / (endTime - startTime);
    }

    // 背景进度条，占满整一行
    Rectangle {
        id: progressBarBackground
        width: parent.width
        height: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: "#E0E0E0"
        radius: 5

        // 中间的活动区域（灰色）
        Rectangle {
            id: activeArea
            width: progressBarBackground.width * (1/3)  // 中间三等分部分
            height: progressBarBackground.height
            x: progressBarBackground.width * (1/3)
            color: "#A39C9C"
            radius: 5

            // 填充进度条（蓝色）
            Rectangle {
                width: activeArea.width * progress
                height: activeArea.height
                color: currentTime >= endTime ? "red" : "#3B99FC"
                radius: 5
            }
        }
    }

    // 开始时间标签，位于进度条的第一个三等分点
    Text {
        text: "检票开始：" + startTimeValue
        anchors.horizontalCenter: parent.left
        anchors.horizontalCenterOffset: parent.width / 3
        anchors.top: progressBarBackground.bottom
        anchors.topMargin: 5
        font.pixelSize: 18
    }

    // 结束时间标签，位于进度条的第三个三等分点
    Text {
        text: "检票结束：" + endTimeValue
        anchors.horizontalCenter: parent.left
        anchors.horizontalCenterOffset: parent.width * 2 / 3
        anchors.top: progressBarBackground.bottom
        anchors.topMargin: 5
        font.pixelSize: 18
    }

    // 当前时间标签
    Text {
        id: currentTimeLabel
        text: currentTime >= endTime ? "已结束检票" : "当前时间：" + currentTimeValue
        font.pixelSize: 18
        color: currentTime >= endTime ? "red" :"blue"
        anchors.verticalCenter: progressBarBackground.top
        anchors.verticalCenterOffset: -25

        // 动态定位
        x: {
            if (currentTime >= endTime) {
                // 如果当前时间超过结束时间，居中显示
                return (parent.width - width) / 2;
            } else {
                var startX = progressBarBackground.x + progressBarBackground.width * (1/3);
                var endX = progressBarBackground.x + progressBarBackground.width * (2/3);
                var pos;
                if (currentTime <= startTime) {
                    // 当前时间早于开始时间
                    pos = activeArea.x - (startTime - currentTime) * (activeArea.width / (startTime - parseTime("00:00")));
                    pos = Math.max(progressBarBackground.x, pos - width / 2);
                } else {
                    // 正常情况下
                    pos = startX + (endX - startX) * progress - width / 2;
                    if (pos < startX) {
                        return startX;
                    } else if (pos + width > endX) {
                        return endX - width;
                    } else {
                        return pos;
                    }
                }
                return pos;
            }
        }
    }

    // 定时器，定期更新当前时间
    Timer {
        interval: 10000  // 每分钟更新一次
        repeat: true
        running: true
        onTriggered: {
            currentTimeValue = Qt.formatTime(new Date(), "HH:mm");
        }
    }
}
