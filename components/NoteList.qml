import QtQuick
import QtQuick.Controls

Flickable {
    id:flickableContainer
    anchors.fill: parent
    contentWidth: width
    contentHeight: Math.min(...yArray)

    property var yArray:[]
    property int column: 2 // 列数，默认为2
    property real spacing: 5


    property bool hasMore: true
    property bool isLoading: false

    property int pageIndex: 1
    // 允许接收一个函数，之后封装好后可以从外面传进来，或者就在这也行
    property var getMoreData:function(loadingCallback, hasMoreCallBack,pageSize=10){
        // 可能需要考虑封装一下网络请求
        const xhr = new XMLHttpRequest();
        xhr.open('POST', 'https://m.ctrip.com/restapi/soa2/16189/json/searchTripShootListForHomePageV2');
        xhr.setRequestHeader('Content-Type', 'application/json');
        const requestBody = JSON.stringify({
                                               "districtId": 152,
                                               "groupChannelCode": "tourphoto_all",
                                               // "locatedDistrictId": 0,
                                               "pagePara": {
                                                   "pageIndex": pageIndex,
                                                   "pageSize": pageSize,
                                                   "sortType": 9,
                                                   "sortDirection": 0
                                               },
                                               // "imageCutType": 1,
                                               "head": {
                                                   // "cid": "09031099210072157423",
                                                   "ctok": "",
                                                   "cver": "1.0",
                                                   "lang": "01",
                                                   "sid": "8888",
                                                   "syscode": "09",
                                                   "auth": "",
                                                   "xsid": "",
                                                   "extension": [
                                                       {
                                                           "name": "source",
                                                           "value": "web"
                                                       },
                                                       {
                                                           "name": "technology",
                                                           "value": "H5"
                                                       },
                                                       {
                                                           "name": "os",
                                                           "value": "PC"
                                                       },
                                                       {
                                                           "name": "application",
                                                           "value": ""
                                                       }
                                                   ]
                                               }
                                           })
        xhr.onreadystatechange = function() {
            if (xhr.readyState === 4) {
                if (xhr.status === 200) {
                    // 处理成功响应
                    const response = JSON.parse(xhr.responseText);
                    // 处理返回的数据
                    const list = response.resultList
                    list.forEach(item=>{
                        listModel.append({article:item.article})
                    })
                    ++pageIndex
                    loadingCallback()
                    if(!response.hasMore){
                        hasMoreCallback()
                    }
                } else {
                    // 处理错误
                    console.error('Request failed');
                    loadCallback()
                }
            }
        };
        xhr.send(requestBody);
    }

    // 根据宽度决定列数
    property var changeColumns:function(windowWidth){
        if(windowWidth<=400){
            return 2
        }else if(windowWidth>400&&windowWidth<=800){
            return 3
        }else{
            return 4
        }
    }

    // 鼠标点击事件, 可能用来处理页面跳转/通信
    property var clickHandler: function(arg){
        console.log(arg)
    }

    MouseArea{
        id:parentMouseArea
        anchors.fill: parent
        propagateComposedEvents: true
        onClicked: {
            if(delegeteTarget){
                clickHandler(delegeteTarget)
            }
        }
        property var delegeteTarget:null
        z:3
    }

    Repeater {
        id: noteContainer
        model:ListModel{
            id:listModel
        }
        delegate: Rectangle{
            objectName: article.articleId
            height: card.height
            width:parent.width/flickableContainer.column - flickableContainer.spacing
            color:"transparent"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    // 以后需要什么返回什么吧
                    parentMouseArea.delegeteTarget = {

                    }
                }
            }

            NoteCard {
                id: card
                Component.onCompleted: {
                    curY = yArray[card.num]
                    yArray[card.num] += card.height + flickableContainer.spacing * 2
                    // 刷新数组, 触发yArray改变
                    yArray = [...yArray]
                }
                width: parent.width
                // 判断第几列
                property int num: index%flickableContainer.column
                x: num * parent.width + 2 * flickableContainer.spacing * num
                property real curY : 0

                // 这个 : 疑似双向绑定, 用curY间接赋值
                y:curY
                note: article
            }
        }

        Component.onCompleted: {
            getMoreData(()=>{},()=>{hasMore=false},16)
        }
    }

    // 滑到底部附近加载数据
    onContentYChanged: {
        if ((contentY >= contentHeight - height - 50) && !isLoading && hasMore) {
            isLoading = true
            getMoreData(() => {isLoading = false},()=>{hasMore=false}, 12)
        }
    }

    Component.onCompleted: {
        column = changeColumns(width)
        yArray.length = column
        yArray.fill(0)
    }

    // 重置
    function reset() {
        if(isLoading){
            return
        }

        const newColumn = changeColumns(width)
        if(newColumn === column){
            return
        }
        isLoading = true
        column = newColumn

        // 重置为0, 设置length多余的会删掉
        yArray.length = newColumn
        yArray.fill(0)

        // 刷新数据, 让页面更新
        noteContainer.model = null
        noteContainer.model = listModel

        isLoading = false
    }

    onWidthChanged: {
        // 可能要考虑做个防抖，但是好像又没啥必要，也就电脑用鼠标移动边框的时候性能有问题
        reset()
    }

}
