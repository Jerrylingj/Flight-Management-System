import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import NetworkHandler 1.0

Page {
    id: flightInfoView

    property string viewName: "航班信息"
    property var flightData: []

    // // 模拟数据
    // property var flightData: [
    //     { "flightNumber": "CA123", "price": 1800, "departure": "北京", "destination": "上海", "departureTime": "08:30", "arrivalTime": "11:00", "remainingSeats": 5, isBooked: true, isFaved: true },
    //     { "flightNumber": "MU456", "price": 2000, "departure": "北京", "destination": "上海", "departureTime": "09:00", "arrivalTime": "11:30", "remainingSeats": 3, isBooked: false, isFaved: false },
    //     { "flightNumber": "HU789", "price": 1500, "departure": "北京", "destination": "上海", "departureTime": "10:00", "arrivalTime": "12:30", "remainingSeats": 10, isBooked: true, isFaved: false },
    //     { "flightNumber": "CA321", "price": 2200, "departure": "北京", "destination": "广州", "departureTime": "07:00", "arrivalTime": "09:30", "remainingSeats": 2, isBooked: false, isFaved: false },
    //     { "flightNumber": "MU654", "price": 1800, "departure": "上海", "destination": "北京", "departureTime": "12:00", "arrivalTime": "14:30", "remainingSeats": 7, isBooked: false, isFaved: true }
    // ]

    // 创建 NetworkHandler 实例
    NetworkHandler {
        id: networkHandler
        onRequestSuccess: function(responseData) {
            var jsonString = JSON.stringify(responseData);
            console.log("请求成功，返回数据：", jsonString); // 打印 JSON 字符串
            flightData = responseData.data;  // 更新 flightData
            updateFilter();  // 重新更新筛选
        }
        onRequestFailed: function(errorMessage) {
            console.log("请求失败：", errorMessage); // 打印失败的错误信息
        }
    }

    // 调用网络请求
    function fetchFlightData() {
        var url = "http://127.0.0.1:8080/api/flights";  // 替换成你实际的后端 API URL
        console.log("发送请求，URL:", url); // 打印请求的 URL
        networkHandler.request(url, NetworkHandler.GET);  // 发送 GET 请求
    }

    // 在页面初始化时调用 fetchFlightData 获取航班数据
    Component.onCompleted: {
        fetchFlightData();  // 页面加载完毕后调用 fetchFlightData 方法获取数据
    }

    // 跟踪当前的排序方法和排序方向
    property int sortMethod: 0  // 默认按时间排序
    property bool isAscending: true  // 默认升序

    ColumnLayout {
        id: flightInfoList
        spacing: 20
        anchors.margins: 20
        width: parent.width

        // 城市选择
        Rectangle {
            color: "#FAFAFA"
            radius: 12
            border.color: "#DDDDDD"
            border.width: 1
            width: parent.width - 40
            height: 80
            anchors.left: parent.left
            anchors.leftMargin: 20 // 左边距
            Layout.topMargin: 10  // 设置上边距为 20 像素

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
                    }
                }

                // 出发城市下拉选择器
                ComboBox {
                    id: departureCity
                    Layout.maximumWidth: 60
                    model: null
                    background: Rectangle {
                        color: "#F9F9F9"
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 1
                    }

                    onCurrentIndexChanged: {
                        arrivalCity.model = getCities(arrivalProvince.currentText)
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
                    }
                }

                // 到达城市下拉选择器
                ComboBox {
                    id: arrivalCity
                    Layout.maximumWidth: 60
                    model: null
                    background: Rectangle {
                        color: "#F9F9F9"
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 1
                    }

                    onCurrentIndexChanged: {
                        arrivalCity.model = getCities(arrivalProvince.currentText)

                    }
                }
            }
        }

        // 筛选 排序
        Rectangle {
            color: "#FAFAFA"
            radius: 12
            border.color: "#DDDDDD"
            border.width: 1
            width: parent.width - 40
            height: 80
            anchors.left: parent.left
            anchors.leftMargin: 20 // 左边距
            Layout.topMargin: 10

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                Layout.fillWidth: true  // 确保 RowLayout 填充整个可用宽度
                spacing: 10

                // 排序选择框
                ComboBox {
                    id: sortMethodComboBox
                    model: [
                        { label: "时间", icon: isAscending ? "↑" : "↓" },
                        { label: "价格", icon: isAscending ? "↑" : "↓" },
                        { label: "座位", icon: isAscending ? "↑" : "↓" }
                    ]
                    Layout.preferredWidth: 55  // 使用 preferredWidth 而不是 maximumWidth
                    font.pixelSize: 16
                    background: Rectangle {
                        color: "#F9F9F9"
                        radius: 10
                        border.color: "#DDDDDD"
                        border.width: 1
                    }
                    textRole: "label"  // 显示 label 属性作为文本
                    onActivated: {
                        sortMethod = currentIndex;
                        toggleSortOrder();
                        updateFilter();
                    }
                    Layout.alignment: Qt.AlignVCenter
                }

                // 筛选按钮
                Button {
                    text: "筛选"
                    font.pixelSize: 16
                    Layout.preferredWidth: 80
                    height: 40
                    background: Rectangle {
                        color: "#007AFF"
                        radius: 12
                        gradient: Gradient {
                            GradientStop { position: 0; color: "#3498DB" }
                            GradientStop { position: 1; color: "#2980B9" }
                        }
                    }
                    onClicked: updateFilter()
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        // 动态展示航班信息
        Repeater {
            model: filteredFlights // 绑定动态筛选后的航班列表
            Loader {
                width: parent.width
                source: "../components/FlightInfoCard.qml"
                property var flightInfo: modelData // 传递每条航班的数据
                onLoaded: {
                    item.flightNumber = flightInfo.flightNumber;
                    item.price = flightInfo.price;
                    item.departure = flightInfo.departure;
                    item.destination = flightInfo.destination;
                    item.departureTime = flightInfo.departureTime;
                    item.arrivalTime = flightInfo.arrivalTime;
                    item.remainingSeats = flightInfo.remainingSeats;
                    item.isBooked = flightInfo.isBooked;
                    item.isFaved = flightInfo.isFaved;
                }
            }
        }
    }

    // 筛选和排序更新函数
    function updateFilter() {
        var searchDeparture = departureCity.text;
        var searchDestination = arrivalCity.text;
        console.log("开始根据城市筛选航班信息：")
        console.log("当前出发城市为 " + searchDeparture + " ，到达城市为 " + searchDestination)

        // 动态筛选航班数据
        filteredFlights = flightData.filter(function(flight) {
            return flight.departure.includes(searchDeparture) && flight.destination.includes(searchDestination);
        });

        // 排序功能
        switch (sortMethod) {
            case 0: // 按时间排序
                filteredFlights.sort(function(a, b) {
                    var timeA = a.departureTime.split(":");
                    var timeB = b.departureTime.split(":");
                    var dateA = new Date(1970, 0, 1, timeA[0], timeA[1]);
                    var dateB = new Date(1970, 0, 1, timeB[0], timeB[1]);
                    return isAscending ? dateA - dateB : dateB - dateA;
                });
                break;
            case 1: // 按价格排序
                filteredFlights.sort(function(a, b) {
                    return isAscending ? a.price - b.price : b.price - a.price;
                });
                break;
            case 2: // 按剩余座位排序
                filteredFlights.sort(function(a, b) {
                    return isAscending ? a.remainingSeats - b.remainingSeats : b.remainingSeats - a.remainingSeats;
                });
                break;
        }

        filteredFlights = filteredFlights.slice();  // 重新赋值给数组，触发数据更新
    }

    // 切换排序顺序
    function toggleSortOrder() {
        isAscending = !isAscending;
        sortMethodComboBox.model = [
            { label: "时间", src: isAscending ? "https://img.icons8.com/?size=100&id=122841&format=png&color=000000" : "https://img.icons8.com/?size=100&id=h2X0Cy3sU70i&format=png&color=000000" },
            { label: "价格", src: isAscending ? "https://img.icons8.com/?size=100&id=122841&format=png&color=000000" : "https://img.icons8.com/?size=100&id=h2X0Cy3sU70i&format=png&color=000000" },
            { label: "座位", src: isAscending ? "https://img.icons8.com/?size=100&id=122841&format=png&color=000000" : "https://img.icons8.com/?size=100&id=h2X0Cy3sU70i&format=png&color=000000" }
        ];
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


    // 初始化时显示所有航班
    property var filteredFlights: flightData
}
