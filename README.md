# Flight-Management-System

A Flight Ticketing Management System Based on QT + MySQL


防止奇怪的事情发生，忽略了build改动               

### 一些事项            
- 导入组件方法            
1. 使用loader         
2. import "../components/"             
> 需要导入某个路径内所有组件而不是某一特定组件，只能导入所有             
- 如果使用了parent.width(或者parent.height)，确保parent的width/height已经设置，否则默认为0.虽然内容会不管父组件宽度而溢出(如果没设置换行), 但是之后想设置边框或者换行时就会出现bug                

### 后端相关
#### 数据库         
- 需要配置ODBC
- 需要在config里新建一个database_config.json文件，database_config.template.json是模板       

#### dto                  
之后可能需要对每个请求的接收信息和返回信息都约定好数据结构，然后放在dto文件夹里。           
命名应该是请求名_dto           
dto尽可能只保存数据结构，不做其它的事情                
`response_dto`是统一的返回格式       

```json
{
  "code":200, // 跟http状态码不同，这个是前后端约定好的，可以是别的。比如状态码为1时表示token过期，前端接收到就执行登出之类的          
  "message":"操作成功", // 如果成功默认为操作成功，失败没有默认值
  "data":"实际获取的数据" // 根据请求的dto不同而不同。如果请求失败了，则为空
}
```
参考一下login和login的dto            
现在的response_dto不是完美的，之后可能要改一下                   


### 文件夹简介

- backend：后端

  - main.cpp：主程序入口，配置各个api的路由
  - database：数据库，含对数据库的增删改查
  - api：各个后端api，调用database中的函数来实现功能
  - dto: 前后端传递信息的约定，表示按照这个格式发送/接收信息
  
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
- 前端逻辑
- 后端逻辑
