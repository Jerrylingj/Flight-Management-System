import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../components/"

// OrdersView.qml
Page {
    // 页面名称属性
    id: ordersView
    property string viewName: '全部订单'

    // 用于页面切换的 StackView
    property StackView stack: StackView.view

    // 模拟数据
    property var orderData:[
        { "flightNumber":"CA123", "departure":"北京", "destination":"上海", "departureTime":"23:00", "arrivalTime":"23:50", "checkInStartTime":"06:30", "checkInEndTime":"07:30" },
        { "flightNumber":"MU456", "departure":"上海", "destination":"广州", "departureTime":"09:30", "arrivalTime":"12:00", "checkInStartTime":"08:00", "checkInEndTime":"08:59" },
        { "flightNumber":"CZ789", "departure":"广州", "destination":"深圳", "departureTime":"11:00", "arrivalTime":"11:45", "checkInStartTime":"09:30", "checkInEndTime":"09:59" },
        { "flightNumber":"HU234", "departure":"深圳", "destination":"成都", "departureTime":"14:00", "arrivalTime":"16:30", "checkInStartTime":"12:30", "checkInEndTime":"13:29" },
        { "flightNumber":"CA345", "departure":"成都", "destination":"杭州", "departureTime":"15:45", "arrivalTime":"18:15", "checkInStartTime":"14:00", "checkInEndTime":"15:14" },
        { "flightNumber":"MU567", "departure":"杭州", "destination":"北京", "departureTime":"07:20", "arrivalTime":"09:40", "checkInStartTime":"05:40", "checkInEndTime":"06:19" },
        { "flightNumber":"CZ890", "departure":"北京", "destination":"广州", "departureTime":"10:00", "arrivalTime":"12:30", "checkInStartTime":"08:30", "checkInEndTime":"08:59" },
        { "flightNumber":"HU678", "departure":"上海", "destination":"深圳", "departureTime":"13:15", "arrivalTime":"15:45", "checkInStartTime":"11:45", "checkInEndTime":"12:44" },
        { "flightNumber":"CA999", "departure":"广州", "destination":"杭州", "departureTime":"16:30", "arrivalTime":"23:50", "checkInStartTime":"15:00", "checkInEndTime":"15:29" },
        { "flightNumber":"MU345", "departure":"深圳", "destination":"上海", "departureTime":"12:00", "arrivalTime":"14:25", "checkInStartTime":"10:30", "checkInEndTime":"10:59" },
        { "flightNumber":"CZ123", "departure":"杭州", "destination":"成都", "departureTime":"17:00", "arrivalTime":"19:30", "checkInStartTime":"15:30", "checkInEndTime":"16:29" },
        { "flightNumber":"HU456", "departure":"成都", "destination":"北京", "departureTime":"06:45", "arrivalTime":"09:15", "checkInStartTime":"05:15", "checkInEndTime":"05:44" },
        { "flightNumber":"CA567", "departure":"北京", "destination":"深圳", "departureTime":"10:30", "arrivalTime":"13:00", "checkInStartTime":"09:00", "checkInEndTime":"09:29" },
        { "flightNumber":"MU678", "departure":"上海", "destination":"杭州", "departureTime":"14:00", "arrivalTime":"15:20", "checkInStartTime":"13:00", "checkInEndTime":"13:59" },
        { "flightNumber":"CZ234", "departure":"广州", "destination":"成都", "departureTime":"15:50", "arrivalTime":"18:10", "checkInStartTime":"14:30", "checkInEndTime":"15:29" },
        { "flightNumber":"HU345", "departure":"深圳", "destination":"北京", "departureTime":"18:00", "arrivalTime":"20:30", "checkInStartTime":"16:30", "checkInEndTime":"17:29" },
        { "flightNumber":"CA890", "departure":"杭州", "destination":"广州", "departureTime":"19:15", "arrivalTime":"21:45", "checkInStartTime":"18:00", "checkInEndTime":"18:44" },
        { "flightNumber":"MU123", "departure":"成都", "destination":"上海", "departureTime":"08:30", "arrivalTime":"11:00", "checkInStartTime":"07:00", "checkInEndTime":"07:29" },
        { "flightNumber":"CZ456", "departure":"北京", "destination":"杭州", "departureTime":"09:45", "arrivalTime":"12:15", "checkInStartTime":"08:00", "checkInEndTime":"08:44" },
        { "flightNumber":"HU789", "departure":"上海", "destination":"成都", "departureTime":"13:50", "arrivalTime":"16:20", "checkInStartTime":"12:30", "checkInEndTime":"13:29" },
        {"flightNumber":"CA1234","departure":"北京","destination":"上海","departureTime":"07:00","arrivalTime":"09:20","checkInStartTime":"06:00","checkInEndTime":"06:45"},
        {"flightNumber":"MU5678","departure":"广州","destination":"深圳","departureTime":"08:00","arrivalTime":"08:45","checkInStartTime":"07:00","checkInEndTime":"07:45"},
        {"flightNumber":"CZ9876","departure":"武汉","destination":"成都","departureTime":"09:00","arrivalTime":"11:00","checkInStartTime":"08:00","checkInEndTime":"08:45"},
        {"flightNumber":"HU4321","departure":"杭州","destination":"西安","departureTime":"10:00","arrivalTime":"12:30","checkInStartTime":"09:00","checkInEndTime":"09:45"},
        {"flightNumber":"CA5678","departure":"深圳","destination":"北京","departureTime":"11:00","arrivalTime":"13:30","checkInStartTime":"10:00","checkInEndTime":"10:45"},
        {"flightNumber":"MU1234","departure":"上海","destination":"广州","departureTime":"12:00","arrivalTime":"14:30","checkInStartTime":"11:00","checkInEndTime":"11:45"},
        {"flightNumber":"CZ5678","departure":"成都","destination":"杭州","departureTime":"13:00","arrivalTime":"15:20","checkInStartTime":"12:00","checkInEndTime":"12:45"},
        {"flightNumber":"HU9876","departure":"西安","destination":"武汉","departureTime":"14:00","arrivalTime":"15:30","checkInStartTime":"13:00","checkInEndTime":"13:45"},
        {"flightNumber":"CA4321","departure":"北京","destination":"深圳","departureTime":"15:00","arrivalTime":"17:30","checkInStartTime":"14:00","checkInEndTime":"14:45"},
        {"flightNumber":"MU9876","departure":"广州","destination":"上海","departureTime":"16:00","arrivalTime":"18:30","checkInStartTime":"15:00","checkInEndTime":"15:45"},
        {"flightNumber":"CZ1234","departure":"深圳","destination":"成都","departureTime":"17:00","arrivalTime":"19:30","checkInStartTime":"16:00","checkInEndTime":"16:45"},
        {"flightNumber":"HU5678","departure":"杭州","destination":"北京","departureTime":"18:00","arrivalTime":"20:30","checkInStartTime":"17:00","checkInEndTime":"17:45"},
        {"flightNumber":"CA9876","departure":"西安","destination":"广州","departureTime":"19:00","arrivalTime":"21:30","checkInStartTime":"18:00","checkInEndTime":"18:45"},
        {"flightNumber":"MU4321","departure":"武汉","destination":"深圳","departureTime":"20:00","arrivalTime":"22:00","checkInStartTime":"19:00","checkInEndTime":"19:45"},
        {"flightNumber":"CZ7654","departure":"成都","destination":"杭州","departureTime":"21:00","arrivalTime":"23:20","checkInStartTime":"20:00","checkInEndTime":"20:45"},
        {"flightNumber":"HU3210","departure":"北京","destination":"西安","departureTime":"22:00","arrivalTime":"00:30","checkInStartTime":"21:00","checkInEndTime":"21:45"},
        {"flightNumber":"CA7654","departure":"上海","destination":"武汉","departureTime":"06:00","arrivalTime":"07:30","checkInStartTime":"05:00","checkInEndTime":"05:45"},
        {"flightNumber":"MU3210","departure":"深圳","destination":"成都","departureTime":"07:00","arrivalTime":"09:30","checkInStartTime":"06:00","checkInEndTime":"06:45"},
        {"flightNumber":"CZ2109","departure":"杭州","destination":"北京","departureTime":"08:00","arrivalTime":"10:30","checkInStartTime":"07:00","checkInEndTime":"07:45"},
        {"flightNumber":"HU6543","departure":"广州","destination":"西安","departureTime":"09:00","arrivalTime":"11:30","checkInStartTime":"08:00","checkInEndTime":"08:45"},
        {"flightNumber":"CA2109","departure":"成都","destination":"上海","departureTime":"10:00","arrivalTime":"12:30","checkInStartTime":"09:00","checkInEndTime":"09:45"},
        {"flightNumber":"MU6543","departure":"西安","destination":"深圳","departureTime":"11:00","arrivalTime":"13:30","checkInStartTime":"10:00","checkInEndTime":"10:45"},
        {"flightNumber":"CZ3210","departure":"武汉","destination":"杭州","departureTime":"12:00","arrivalTime":"14:00","checkInStartTime":"11:00","checkInEndTime":"11:45"},
        {"flightNumber":"HU7654","departure":"北京","destination":"广州","departureTime":"13:00","arrivalTime":"15:30","checkInStartTime":"12:00","checkInEndTime":"12:45"},
        {"flightNumber":"CA3210","departure":"上海","destination":"成都","departureTime":"14:00","arrivalTime":"16:30","checkInStartTime":"13:00","checkInEndTime":"13:45"},
        {"flightNumber":"MU2109","departure":"深圳","destination":"西安","departureTime":"15:00","arrivalTime":"17:30","checkInStartTime":"14:00","checkInEndTime":"14:45"},
        {"flightNumber":"CZ6543","departure":"广州","destination":"北京","departureTime":"16:00","arrivalTime":"18:30","checkInStartTime":"15:00","checkInEndTime":"15:45"},
        {"flightNumber":"HU2109","departure":"成都","destination":"武汉","departureTime":"17:00","arrivalTime":"19:00","checkInStartTime":"16:00","checkInEndTime":"16:45"},
        {"flightNumber":"CA6543","departure":"杭州","destination":"深圳","departureTime":"18:00","arrivalTime":"20:30","checkInStartTime":"17:00","checkInEndTime":"17:45"},
        {"flightNumber":"MU7654","departure":"西安","destination":"上海","departureTime":"19:00","arrivalTime":"21:30","checkInStartTime":"18:00","checkInEndTime":"18:45"},
        {"flightNumber":"CZ4321","departure":"北京","destination":"成都","departureTime":"20:00","arrivalTime":"22:30","checkInStartTime":"19:00","checkInEndTime":"19:45"},
        {"flightNumber":"HU1234","departure":"广州","destination":"杭州","departureTime":"21:00","arrivalTime":"23:20","checkInStartTime":"20:00","checkInEndTime":"20:45"},
        {"flightNumber":"CA7654","departure":"深圳","destination":"武汉","departureTime":"22:00","arrivalTime":"23:30","checkInStartTime":"21:00","checkInEndTime":"21:45"},
        {"flightNumber":"MU8765","departure":"成都","destination":"西安","departureTime":"23:00","arrivalTime":"01:30","checkInStartTime":"22:00","checkInEndTime":"22:45"},
        {"flightNumber":"CZ9876","departure":"北京","destination":"广州","departureTime":"07:00","arrivalTime":"09:20","checkInStartTime":"06:00","checkInEndTime":"06:45"},
        {"flightNumber":"HU5432","departure":"上海","destination":"深圳","departureTime":"08:00","arrivalTime":"10:30","checkInStartTime":"07:00","checkInEndTime":"07:45"},
        {"flightNumber":"CA4321","departure":"杭州","destination":"成都","departureTime":"09:00","arrivalTime":"11:20","checkInStartTime":"08:00","checkInEndTime":"08:45"},
        {"flightNumber":"MU3210","departure":"西安","destination":"武汉","departureTime":"10:00","arrivalTime":"11:30","checkInStartTime":"09:00","checkInEndTime":"09:45"},
        {"flightNumber":"CZ2109","departure":"广州","destination":"北京","departureTime":"11:00","arrivalTime":"13:30","checkInStartTime":"10:00","checkInEndTime":"10:45"},
        {"flightNumber":"HU6543","departure":"成都","destination":"上海","departureTime":"12:00","arrivalTime":"14:30","checkInStartTime":"11:00","checkInEndTime":"11:45"},
        {"flightNumber":"CA1234","departure":"深圳","destination":"杭州","departureTime":"13:00","arrivalTime":"15:30","checkInStartTime":"12:00","checkInEndTime":"12:45"},
        {"flightNumber":"MU7654","departure":"武汉","destination":"西安","departureTime":"14:00","arrivalTime":"15:30","checkInStartTime":"13:00","checkInEndTime":"13:45"},
        {"flightNumber":"CZ8765","departure":"北京","destination":"成都","departureTime":"15:00","arrivalTime":"17:30","checkInStartTime":"14:00","checkInEndTime":"14:45"},
        {"flightNumber":"HU9876","departure":"广州","destination":"深圳","departureTime":"16:00","arrivalTime":"16:45","checkInStartTime":"15:00","checkInEndTime":"15:45"},
        {"flightNumber":"CA5432","departure":"上海","destination":"武汉","departureTime":"17:00","arrivalTime":"18:30","checkInStartTime":"16:00","checkInEndTime":"16:45"},
        {"flightNumber":"MU6543","departure":"成都","destination":"北京","departureTime":"18:00","arrivalTime":"20:30","checkInStartTime":"17:00","checkInEndTime":"17:45"},
        {"flightNumber":"CZ7654","departure":"深圳","destination":"西安","departureTime":"19:00","arrivalTime":"21:30","checkInStartTime":"18:00","checkInEndTime":"18:45"},
        {"flightNumber":"HU4321","departure":"杭州","destination":"广州","departureTime":"20:00","arrivalTime":"22:30","checkInStartTime":"19:00","checkInEndTime":"19:45"},
        {"flightNumber":"CA3210","departure":"武汉","destination":"成都","departureTime":"21:00","arrivalTime":"23:00","checkInStartTime":"20:00","checkInEndTime":"20:45"},
        {"flightNumber":"MU2109","departure":"北京","destination":"上海","departureTime":"22:00","arrivalTime":"00:20","checkInStartTime":"21:00","checkInEndTime":"21:45"},
        {"flightNumber":"CZ1234","departure":"广州","destination":"深圳","departureTime":"23:00","arrivalTime":"23:45","checkInStartTime":"22:00","checkInEndTime":"22:45"}
    ]

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
                        item.checkInStartTime = orderInfo.checkInStartTime;
                        item.checkInEndTime = orderInfo.checkInEndTime;
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
