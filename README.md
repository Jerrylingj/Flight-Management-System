# Flight-Management-System
A Flight Ticketing Management System Based on QT + MySQL


防止奇怪的事情发生，忽略了build改动

### 文件夹简介        
- components: 存放各种组件
- views: 存放页面
- api 存放与服务器交互的代码
- figure 存放图片         

### 命名规范
对于qml, cpp文件应该使用大驼峰命名，如NoteCard.qml, NetworkHandler.cpp.            
页面应该以View作为结尾，并放在views文件夹中, 如HomeView.qml           
图片应该放在figure中相应的文件夹。比如login页面需要用到flight.png, 则应该将其放在/figure/login中            
qml中自定义属性以及函数使用小驼峰命名，尽量不使用拼音              
编写完成后使用Qt自带的代码格式化工具 Ctrl+I 来统一缩进。~~虽然QT的缩进真的一坨~~          
