import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import NetworkHandler 1.0

import "../components/"

// OrdersView.qml
Page {
    // 页面名称属性
    id: ordersView
    property string viewName: '全部订单'

    // 用于页面切换的 StackView
    property StackView stack: StackView.view

    // 模拟数据
    /*
    property var orderData:[
        {
          "flightNumber": "CA123",
          "departure": "北京",
          "destination": "上海",
          "departureTime": "07:00",
          "arrivalTime": "09:50",
          "departureAirport": "北京大兴国际机场",
          "arrivalAirport": "上海虹桥国际机场",
          "checkInStartTime": "04:30",
          "checkInEndTime": "05:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA124",
          "departure": "北京",
          "destination": "广州",
          "departureTime": "08:00",
          "arrivalTime": "11:00",
          "departureAirport": "北京首都国际机场",
          "arrivalAirport": "广州白云国际机场",
          "checkInStartTime": "05:30",
          "checkInEndTime": "06:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA125",
          "departure": "北京",
          "destination": "深圳",
          "departureTime": "09:00",
          "arrivalTime": "12:00",
          "departureAirport": "北京大兴国际机场",
          "arrivalAirport": "深圳宝安国际机场",
          "checkInStartTime": "06:30",
          "checkInEndTime": "07:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA126",
          "departure": "上海",
          "destination": "北京",
          "departureTime": "10:00",
          "arrivalTime": "12:50",
          "departureAirport": "上海虹桥国际机场",
          "arrivalAirport": "北京大兴国际机场",
          "checkInStartTime": "07:30",
          "checkInEndTime": "08:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA127",
          "departure": "上海",
          "destination": "广州",
          "departureTime": "11:00",
          "arrivalTime": "13:50",
          "departureAirport": "上海浦东国际机场",
          "arrivalAirport": "广州白云国际机场",
          "checkInStartTime": "08:30",
          "checkInEndTime": "09:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA128",
          "departure": "上海",
          "destination": "深圳",
          "departureTime": "12:00",
          "arrivalTime": "14:50",
          "departureAirport": "上海虹桥国际机场",
          "arrivalAirport": "深圳宝安国际机场",
          "checkInStartTime": "09:30",
          "checkInEndTime": "10:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA129",
          "departure": "广州",
          "destination": "北京",
          "departureTime": "13:00",
          "arrivalTime": "16:00",
          "departureAirport": "广州白云国际机场",
          "arrivalAirport": "北京首都国际机场",
          "checkInStartTime": "10:30",
          "checkInEndTime": "11:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA130",
          "departure": "广州",
          "destination": "上海",
          "departureTime": "14:00",
          "arrivalTime": "16:50",
          "departureAirport": "广州白云国际机场",
          "arrivalAirport": "上海浦东国际机场",
          "checkInStartTime": "11:30",
          "checkInEndTime": "12:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA131",
          "departure": "广州",
          "destination": "深圳",
          "departureTime": "15:00",
          "arrivalTime": "16:30",
          "departureAirport": "广州白云国际机场",
          "arrivalAirport": "深圳宝安国际机场",
          "checkInStartTime": "12:30",
          "checkInEndTime": "13:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA132",
          "departure": "深圳",
          "destination": "北京",
          "departureTime": "16:00",
          "arrivalTime": "19:00",
          "departureAirport": "深圳宝安国际机场",
          "arrivalAirport": "北京大兴国际机场",
          "checkInStartTime": "13:30",
          "checkInEndTime": "14:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA133",
          "departure": "深圳",
          "destination": "上海",
          "departureTime": "17:00",
          "arrivalTime": "19:50",
          "departureAirport": "深圳宝安国际机场",
          "arrivalAirport": "上海虹桥国际机场",
          "checkInStartTime": "14:30",
          "checkInEndTime": "15:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA134",
          "departure": "深圳",
          "destination": "广州",
          "departureTime": "18:00",
          "arrivalTime": "19:30",
          "departureAirport": "深圳宝安国际机场",
          "arrivalAirport": "广州白云国际机场",
          "checkInStartTime": "15:30",
          "checkInEndTime": "16:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA135",
          "departure": "北京",
          "destination": "深圳",
          "departureTime": "19:00",
          "arrivalTime": "22:00",
          "departureAirport": "北京大兴国际机场",
          "arrivalAirport": "深圳宝安国际机场",
          "checkInStartTime": "16:30",
          "checkInEndTime": "17:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA136",
          "departure": "北京",
          "destination": "广州",
          "departureTime": "20:00",
          "arrivalTime": "23:00",
          "departureAirport": "北京首都国际机场",
          "arrivalAirport": "广州白云国际机场",
          "checkInStartTime": "17:30",
          "checkInEndTime": "18:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA137",
          "departure": "北京",
          "destination": "上海",
          "departureTime": "21:00",
          "arrivalTime": "23:50",
          "departureAirport": "北京大兴国际机场",
          "arrivalAirport": "上海虹桥国际机场",
          "checkInStartTime": "18:30",
          "checkInEndTime": "19:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA138",
          "departure": "上海",
          "destination": "深圳",
          "departureTime": "22:00",
          "arrivalTime": "00:50",
          "departureAirport": "上海虹桥国际机场",
          "arrivalAirport": "深圳宝安国际机场",
          "checkInStartTime": "19:30",
          "checkInEndTime": "20:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA139",
          "departure": "上海",
          "destination": "广州",
          "departureTime": "23:00",
          "arrivalTime": "01:50",
          "departureAirport": "上海浦东国际机场",
          "arrivalAirport": "广州白云国际机场",
          "checkInStartTime": "20:30",
          "checkInEndTime": "21:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA140",
          "departure": "上海",
          "destination": "北京",
          "departureTime": "00:00",
          "arrivalTime": "02:50",
          "departureAirport": "上海虹桥国际机场",
          "arrivalAirport": "北京大兴国际机场",
          "checkInStartTime": "21:30",
          "checkInEndTime": "22:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA141",
          "departure": "广州",
          "destination": "深圳",
          "departureTime": "01:00",
          "arrivalTime": "02:30",
          "departureAirport": "广州白云国际机场",
          "arrivalAirport": "深圳宝安国际机场",
          "checkInStartTime": "22:30",
          "checkInEndTime": "23:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA142",
          "departure": "广州",
          "destination": "上海",
          "departureTime": "02:00",
          "arrivalTime": "04:50",
          "departureAirport": "广州白云国际机场",
          "arrivalAirport": "上海浦东国际机场",
          "checkInStartTime": "23:30",
          "checkInEndTime": "00:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA143",
          "departure": "广州",
          "destination": "北京",
          "departureTime": "03:00",
          "arrivalTime": "06:00",
          "departureAirport": "广州白云国际机场",
          "arrivalAirport": "北京首都国际机场",
          "checkInStartTime": "00:30",
          "checkInEndTime": "01:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA144",
          "departure": "深圳",
          "destination": "广州",
          "departureTime": "04:00",
          "arrivalTime": "05:30",
          "departureAirport": "深圳宝安国际机场",
          "arrivalAirport": "广州白云国际机场",
          "checkInStartTime": "01:30",
          "checkInEndTime": "02:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA145",
          "departure": "深圳",
          "destination": "上海",
          "departureTime": "05:00",
          "arrivalTime": "07:50",
          "departureAirport": "深圳宝安国际机场",
          "arrivalAirport": "上海虹桥国际机场",
          "checkInStartTime": "02:30",
          "checkInEndTime": "03:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA146",
          "departure": "深圳",
          "destination": "北京",
          "departureTime": "06:00",
          "arrivalTime": "09:00",
          "departureAirport": "深圳宝安国际机场",
          "arrivalAirport": "北京大兴国际机场",
          "checkInStartTime": "03:30",
          "checkInEndTime": "04:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA147",
          "departure": "北京",
          "destination": "深圳",
          "departureTime": "07:00",
          "arrivalTime": "10:00",
          "departureAirport": "北京大兴国际机场",
          "arrivalAirport": "深圳宝安国际机场",
          "checkInStartTime": "04:30",
          "checkInEndTime": "05:30",
          "airlineCompany": "东方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA148",
          "departure": "北京",
          "destination": "广州",
          "departureTime": "08:00",
          "arrivalTime": "11:00",
          "departureAirport": "北京首都国际机场",
          "arrivalAirport": "广州白云国际机场",
          "checkInStartTime": "05:30",
          "checkInEndTime": "06:30",
          "airlineCompany": "南方航空",
          "status": "OnTime"
        },
        {
          "flightNumber": "CA149",
          "departure": "北京",
          "destination": "上海",
          "departureTime": "09:00",
          "arrivalTime": "11:50",
          "departureAirport": "北京大兴国际机场",
          "arrivalAirport": "上海虹桥国际机场",
          "checkInStartTime": "06:30",
          "checkInEndTime": "07:30",
          "airlineCompany": "中国国航",
          "status": "OnTime"
        }
      ]
    */

    // 数据库传入的数据
    property var orderData: []

    // 创建 NetworkHandler 实例
    NetworkHandler {
        id: networkHandler

        onRequestSuccess: function(responseData) {
            var jsonString = JSON.stringify(responseData);
            console.log("请求成功，返回数据：", jsonString); // 打印 JSON 字符串

            // 检查 responseData 是否为数组
            if (Array.isArray(responseData)) {
                console.log("responseData 是一个数组，长度为:", responseData.length);
                // 为每个航班添加 isBooked 和 isFaved 字段，初始化为 false
                orderData = responseData.map(function(order) {
                    // 需要根据航班号查询数据并加入
                    return order;
                });
            } else {
                console.log("responseData 不是一个数组，类型为:", typeof responseData);
                // 如果 responseData 不是数组，检查是否包含数组字段
                if (responseData.data && Array.isArray(responseData.data)) {
                    console.log("responseData.data 是一个数组，长度为:", responseData.data.length);
                    orderData = responseData.data.map(function(order) {
                        // 需要根据航班号查询数据并加入
                        return order;
                    });

                } else {
                    console.log("无法识别的响应数据结构");
                    orderData = [];
                }
            }

            updateFilter();  // 重新更新筛选
        }

        onRequestFailed: function(errorMessage) {
            console.log("请求失败：", errorMessage); // 打印失败的错误信息
        }
    }


    // 调用网络请求
    function fetchOrderData() {
        const url = "/api/orders";  // 后端 API URL
        console.log("发送获取全部订单信息的请求，URL:", url); // 打印请求的 URL
        networkHandler.request(url, NetworkHandler.GET);  // 发送 GET 请求
    }

    // 在页面初始化时调用 fetchFlightData 获取航班数据
    Component.onCompleted: {
        fetchOrderData();  // 页面加载完毕后调用 fetchOrderData 方法获取数据
    }

    // 筛选器
    property bool isAscending: true
    Rectangle{
        id: orderFilter
        color : "#FAFAFA"
        radius: 0
        border.width: 0
        width : parent.width
        height : 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        z : 2

        Rectangle {
            id : orderFilterRectangle
            color: "#FAFAFA"
            radius: 12
            border.color: "#DDDDDD"
            border.width: 1
            width: parent.width - 40
            height: 80
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            // anchors.bottom : parent.top + height + 20

            // anchors.topMargin: 20  // 顶部边距

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                Layout.fillWidth: true  // 确保 RowLayout 填充整个可用宽度
                spacing: 10
                // 出发省份下拉选择器
                ComboBox {
                    id: departureProvince
                    Layout.maximumWidth: 60
                    model: ["全部省份", "北京", "上海", "天津", "重庆", "河北", "山西", "内蒙古", "辽宁", "吉林", "黑龙江", "江苏", "浙江", "安徽", "福建", "江西", "山东", "河南", "湖北", "湖南", "广东", "广西", "海南", "四川", "贵州", "云南", "西藏", "陕西", "甘肃", "青海", "宁夏", "新疆", "香港", "澳门", "台湾"]
                    background: Rectangle {
                        color: "#F9F9F9"
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 1
                    }

                    onCurrentTextChanged: {
                        // 更新出发城市列表并重置选择
                        departureCity.model = getCities(departureProvince.currentText)
                        departureCity.currentIndex = 0  // 重置城市选择为第一个
                        console.log("出发省份已更新为 " + currentText)
                        updateFilter()
                    }
                }

                // 出发城市下拉选择器
                ComboBox {
                    id: departureCity
                    Layout.maximumWidth: 60
                    model: getCities(departureProvince.currentText)  // 初始化城市列表
                    background: Rectangle {
                        color: "#F9F9F9"
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 1
                    }

                    onCurrentTextChanged: {
                        console.log("出发城市已更新为 " + currentText)
                        updateFilter()
                    }
                }

                Text {
                    text: "————"
                    width: 20
                }

                // 到达省份下拉选择器
                ComboBox {
                    id: arrivalProvince
                    Layout.maximumWidth: 60
                    model: ["全部省份", "北京", "上海", "天津", "重庆", "河北", "山西", "内蒙古", "辽宁", "吉林", "黑龙江", "江苏", "浙江", "安徽", "福建", "江西", "山东", "河南", "湖北", "湖南", "广东", "广西", "海南", "四川", "贵州", "云南", "西藏", "陕西", "甘肃", "青海", "宁夏", "新疆", "香港", "澳门", "台湾"]
                    background: Rectangle {
                        color: "#F9F9F9"
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 1
                    }

                    onCurrentTextChanged: {
                        // 更新到达城市列表并重置选择
                        arrivalCity.model = getCities(arrivalProvince.currentText)
                        arrivalCity.currentIndex = 0 // 重置城市选择为第一个
                        console.log("到达省份已更新为 " + currentText)
                        updateFilter();
                    }
                }
                // 到达城市下拉选择器
                ComboBox {
                    id: arrivalCity
                    Layout.maximumWidth: 60
                    model: getCities(arrivalProvince.currentText)
                    background: Rectangle {
                        color: "#F9F9F9"
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 1
                    }

                    onCurrentTextChanged: {
                        console.log("到达城市已更新为 " + currentText)
                        updateFilter();
                    }
                }
            }
        }
    }


    // 卡片展示区域
    // 使用 Flickable 来替代 ScrollView，允许用户滚动浏览内容
    Flickable {
        anchors {
            left: parent.left
            right: parent.right
            top: orderFilter.bottom
            bottom: parent.bottom
            topMargin : 20
        }

        contentWidth: parent.width
        contentHeight: orderInfoList.height
        flickableDirection: Flickable.VerticalFlick

        ColumnLayout {
            id: orderInfoList
            // anchors.fill: parent
            spacing: 20
            anchors.topMargin: 20  // 替代 padding
            width: parent.width

            Repeater {
                model: filteredOrders
                Loader {
                    // 用于传递和更新订单详细信息的属性
                    property var info
                    // info : info

                    // 当 info 发生变化时，切换到订单详情页面
                    onInfoChanged: {
                        stack.changeTo("views/OrderDetailView.qml", info)
                    }

                    width : parent.width
                    source: "../components/OrderInfoCard.qml"
                    property var orderInfo : modelData
                    onLoaded: {
                        item.flightNumber = orderInfo.flightNumber;
                        item.departure = orderInfo.departure;
                        item.destination = orderInfo.destination;
                        item.departureTime = orderInfo.departureTime;
                        item.arrivalTime = orderInfo.arrivalTime;
                        item.departureAirport = orderInfo.departureAirport;
                        item.arrivalAirport = orderInfo.arrivalAirport;
                        item.checkInStartTime = orderInfo.checkInStartTime;
                        item.checkInEndTime = orderInfo.checkInEndTime;
                        item.airlineCompany = orderInfo.airlineCompany;
                        item.status = orderInfo.status;
                    }
                }
            }
        }
    }

    // Helper function to获取城市列表
    function getCities(province) {
        var cities = {
            "全部省份": ["全部城市"],
            "北京": ["北京"],
            "上海": ["上海"],
            "天津": ["天津"],
            "重庆": ["重庆"],
            "河北": ["石家庄", "唐山", "秦皇岛", "邯郸", "邢台", "保定", "张家口", "承德", "沧州", "廊坊", "衡水"],
            "山西": ["太原", "大同", "阳泉", "长治", "晋城", "朔州", "晋中", "运城", "忻州", "临汾", "吕梁"],
            "内蒙古": ["呼和浩特", "包头", "乌海", "赤峰", "通辽", "鄂尔多斯", "呼伦贝尔", "巴彦淖尔", "乌兰察布", "兴安盟", "锡林郭勒盟", "阿拉善盟"],
            "辽宁": ["沈阳", "大连", "鞍山", "抚顺", "本溪", "丹东", "锦州", "营口", "阜新", "辽阳", "盘锦", "铁岭", "朝阳", "葫芦岛"],
            "吉林": ["长春", "吉林", "四平", "辽源", "通化", "白山", "松原", "白城", "延边朝鲜族自治州"],
            "黑龙江": ["哈尔滨", "齐齐哈尔", "鸡西", "鹤岗", "双鸭山", "大庆", "伊春", "佳木斯", "七台河", "牡丹江", "黑河", "绥化", "大兴安岭地区"],
            "江苏": ["南京", "无锡", "徐州", "常州", "苏州", "南通", "连云港", "淮安", "盐城", "扬州", "镇江", "泰州", "宿迁"],
            "浙江": ["杭州", "宁波", "温州", "嘉兴", "湖州", "绍兴", "金华", "衢州", "舟山", "台州", "丽水"],
            "安徽": ["合肥", "芜湖", "蚌埠", "淮南", "马鞍山", "淮北", "铜陵", "安庆", "黄山", "滁州", "阜阳", "宿州", "六安", "亳州", "池州", "宣城"],
            "福建": ["福州", "厦门", "莆田", "三明", "泉州", "漳州", "南平", "龙岩", "宁德"],
            "江西": ["南昌", "景德镇", "萍乡", "九江", "新余", "鹰潭", "赣州", "吉安", "宜春", "抚州", "上饶"],
            "山东": ["济南", "青岛", "淄博", "枣庄", "东营", "烟台", "潍坊", "济宁", "泰安", "威海", "日照", "临沂", "德州", "聊城", "滨州", "菏泽"],
            "河南": ["郑州", "开封", "洛阳", "平顶山", "安阳", "鹤壁", "新乡", "焦作", "濮阳", "许昌", "漯河", "三门峡", "南阳", "商丘", "信阳", "周口", "驻马店", "济源"],
            "湖北": ["武汉", "黄石", "十堰", "宜昌", "襄阳", "鄂州", "荆门", "孝感", "荆州", "黄冈", "咸宁", "随州", "恩施土家族苗族自治州"],
            "湖南": ["长沙", "株洲", "湘潭", "衡阳", "邵阳", "岳阳", "常德", "张家界", "益阳", "郴州", "永州", "怀化", "娄底", "湘西土家族苗族自治州"],
            "广东": ["广州", "深圳", "珠海", "汕头", "佛山", "韶关", "湛江", "肇庆", "江门", "茂名", "惠州", "梅州", "汕尾", "河源", "阳江", "清远", "东莞", "中山", "潮州", "揭阳", "云浮"],
            "广西": ["南宁", "柳州", "桂林", "梧州", "北海", "防城港", "钦州", "贵港", "玉林", "百色", "贺州", "河池", "来宾", "崇左"],
            "海南": ["海口", "三亚", "三沙", "儋州", "琼海", "文昌", "万宁", "东方", "定安县", "屯昌县", "澄迈县", "临高县", "白沙黎族自治县", "昌江黎族自治县", "乐东黎族自治县", "陵水黎族自治县", "保亭黎族苗族自治县", "琼中黎族苗族自治县"],
            "四川": ["成都", "自贡", "攀枝花", "泸州", "德阳", "绵阳", "广元", "遂宁", "内江", "乐山", "南充", "眉山", "宜宾", "广安", "达州", "雅安", "巴中", "资阳", "阿坝藏族羌族自治州", "甘孜藏族自治州", "凉山彝族自治州"],
            "贵州": ["贵阳", "六盘水", "遵义", "安顺", "毕节", "铜仁", "黔西南布依族苗族自治州", "黔东南苗族侗族自治州", "黔南布依族苗族自治州"],
            "云南": ["昆明", "曲靖", "玉溪", "保山", "昭通", "丽江", "普洱", "临沧", "楚雄彝族自治州", "红河哈尼族彝族自治州", "文山壮族苗族自治州", "西双版纳傣族自治州", "大理白族自治州", "德宏傣族景颇族自治州", "怒江傈僳族自治州", "迪庆藏族自治州"],
            "西藏": ["拉萨", "日喀则", "昌都", "林芝", "山南", "那曲", "阿里地区"],
            "陕西": ["西安", "铜川", "宝鸡", "咸阳", "渭南", "延安", "汉中", "榆林", "安康", "商洛"],
            "甘肃": ["兰州", "嘉峪关", "金昌", "白银", "天水", "武威", "张掖", "平凉", "酒泉", "庆阳", "定西", "陇南", "临夏回族自治州", "甘南藏族自治州"],
            "青海": ["西宁", "海东", "海北藏族自治州", "黄南藏族自治州", "海南藏族自治州", "果洛藏族自治州", "玉树藏族自治州", "海西蒙古族藏族自治州"],
            "宁夏": ["银川", "石嘴山", "吴忠", "固原", "中卫"],
            "新疆": ["乌鲁木齐", "克拉玛依", "吐鲁番", "哈密", "昌吉回族自治州", "博尔塔拉蒙古自治州", "巴音郭楞蒙古自治州", "阿克苏地区", "克孜勒苏柯尔克孜自治州", "喀什地区", "和田地区", "伊犁哈萨克自治州", "塔城地区", "阿勒泰地区"],
            "香港": ["香港"],
            "澳门": ["澳门"],
            "台湾": ["台北", "高雄", "台中", "台南", "新北", "桃园", "基隆", "新竹", "嘉义"]
        };
        return cities[province] || []
    }

    // Helper function to筛选航班
    function updateFilter() {
        console.log("开始根据城市筛选订单信息：")
        var searchDeparture = departureCity.currentText;
        var searchDestination = arrivalCity.currentText;
        console.log("当前出发城市为 " + searchDeparture + " ，到达城市为 " + searchDestination)

        filteredOrders = orderData.filter(function(order){
            if(searchDeparture !== "全部城市" && searchDestination !== "全部城市"){
                // console.log("出发城市筛选+到达城市筛选")
                return order.departure.includes(searchDeparture) && order.destination.includes(searchDestination);
            }else if(searchDeparture !== "全部城市"){
                // console.log("出发城市筛选")
                return order.departure.includes(searchDeparture);
            }else if(searchDestination !== "全部城市"){
                // console.log("到达城市筛选")
                return order.destination.includes(searchDestination);
            }else{
                return true;
            }
        });

        console.log("尝试更新filteredOrders")
        // filteredOrders = filteredOrders.slice();
    }

    property var filteredOrders : orderData
}
