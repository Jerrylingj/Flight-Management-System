# Flight-Management-System

A Flight Ticketing Management System Based on QT + MySQL


防止奇怪的事情发生，忽略了build改动               

### 一些事项            
- 导入组件方法            
1. 使用loader         
2. import "../components/"             
> 需要导入某个路径内所有组件而不是某一特定组件，只能导入所有             
- 如果使用了parent.width(或者parent.height)，确保parent的width/height已经设置，否则默认为0.虽然内容会不管父组件宽度而溢出(如果没设置换行), 但是之后想设置边框或者换行时就会出现bug                


### 文件夹简介        

- backend：后端

  - main.cpp：主程序入口
  - routes：API相关路由
  - controllers：对接前端接口
  - models：对数据库操作
  - utils：一些工具

- frontend：前端

  - main.qml：主页面入口
  - components：各种组件

  - views：存放页面

  - assets：图片、样式等静态资源
    - figure：图片     


  - utils：一些工具，如js的网络请求函数



### 命名规范

对于 qml , cpp 文件应该使用大驼峰命名，如 NoteCard.qml  ,  NetworkHandler.cpp           
页面应该以 View 作为结尾，并放在 views 文件夹中, 如HomeView.qml           
图片应该放在 figure 中相应的文件夹。比如 login 页面需要用到flight.png , 则应该将其放在 /figure/login 中            
qml中自定义属性以及函数使用小驼峰命名，尽量不使用拼音              
编写完成后使用Qt自带的代码格式化工具 Ctrl+I 来统一缩进。~~虽然QT的缩进真的一坨~~         
### 还没有完成的任务
- 我的订单页面
- 登录、注册、更改个人信息页面
- 前端逻辑
- 后端逻辑
