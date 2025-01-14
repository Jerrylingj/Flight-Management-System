# 🌍 云途 - 后端仓库

[English Version](README.md) | [简体中文](README-zh.md)

---

<div align="center">
  <img src="./favicon.jpg" alt="Altair Backend Logo" style="border-radius: 8px;"/>
</div>


---

## 🚀 **系统介绍**

**云途** 是一个全栈航班管理系统的后端模块，由 **[终端露台](https://github.com/Terminal-Terrace)** 团队开发，基于 **C++** 和 **MySQL** 构建，旨在提供高效可靠的 API 服务，支持用户端和管理员端的操作，包括：

- **用户管理**。
- **航班管理**。
- **订单管理**。
- **数据交互与安全机制**。

---

## 🖂 **项目结构**

```plaintext
├── backend/                 # 后端核心模块
│   ├── aichat/              # 人工客服模块，提供智能客服服务
│   ├── api/                 # API 路由和逻辑处理模块，调用 database 中的函数实现具体功能
│   ├── config/              # 全局配置文件夹，存储项目的配置文件（如数据库配置）
│   ├── database/            # 数据库操作模块，封装对数据库的增删改查逻辑
│   ├── doc/                 # 项目相关文档，包含 API 接口说明和数据结构定义
│   ├── dto/                 # 数据传输对象 (DTO)，定义前后端信息传递的统一数据结构
│   ├── util/                # 工具模块，包含密码加解密、网络请求封装等通用函数
│   ├── main.cpp             # 后端程序的主入口，负责初始化服务并配置 API 路由
│   └── CMakeLists.txt       # CMake 构建脚本，定义项目构建规则

├── fetchFlights/            # 航班信息动态爬取模块，定期更新航班数据
├── frontend/                # 废弃的移动端前端代码（历史版本）
├── microBackend/            # 微后端模块，提供邮箱注册与登录功能
└── README.md                # 项目说明文档
```

## ⚙️ **开发环境**

| 要求          | 详情            |
| ------------- | --------------- |
| **C++版本**   | C++17           |
| **MySQL版本** | 8.0+            |
| **构建工具**  | CMake 3.15      |
| **操作系统**  | Windows / Linux |

------

## 🛠️ **开始使用**

### 1. 克隆项目

```Bash
git clone git@github.com:Jerrylingj/Flight-Management-System.git
```

### 2. 配置 MySQL 和 ODBC

1. 安装并配置 **MySQL 数据库**。

2. 配置 ODBC 驱动以支持数据库连接。

3. 在 `backend/config/`和`fetchFlights`目录下创建 `database_config.json`文件，参考模板 `database_config.template.json`：

   ```json
   {
       "hostname": "127.0.0.1",
       "port": 3306,
       "database": "your_database",
       "username": "your_username",
       "password": "your_password"
   }
   ```

### 3. 启动航班信息动态爬取模块

1. 运行 `fetchFlights/get_flights.exe` 文件，执行爬取脚本。

2. 执行爬取脚本，将数据写入数据库，运行以下命令：

   ```bash
   node main.js
   ```

### 4. 配置邮箱注册与登录模块

1. 在`microBackend/`目录下创建`smtp.json`文件，参考模板`smtp.template.json`:

```json
{
    "host": "smtp.qq.com",
    "port": 465,
    "secure": true,
    "user": "your-email@qq.com",
    "pass": "your-smtp-auth-code"
}
```

2. 启动服务：

```bash
node app.js
```

3. 如果配置成功，您将看到以下提示：

```bash
Server is running on port 3000
Reading SMTP configuration...
Server is ready to send emails
```

### 5. 配置AI客服



------

## ✨ **功能概览**

### 核心功能

- **用户管理**：支持用户注册、登录，提供账户信息管理功能。
- **航班管理**：支持管理员添加、修改、删除航班信息。
- **订单管理**：实现订单创建、支付、状态查询等功能。
- **人工客服**：集成人工智能客服模块，提升用户体验。

### 设计亮点

- **模块化设计**：项目采用分层架构，逻辑清晰，易于维护。
- **数据安全**：用户数据使用 AES256 加密存储，支付接口采用 SSL/TLS 加密。
- **高扩展性**：数据库支持分布式架构，系统性能优化良好。

------

## 🛠️ **注意事项**

### 数据传输对象（DTO）

- **定义**：所有请求与响应的传输数据需符合 DTO 规范，统一在 `dto/` 文件夹中管理。

- **统一返回格式**：

  ```json
  {
    "code": 200, 					// 跟http状态码不同，这个是前后端约定好的，可以是别的。比如状态码为1时表示token过期，前端接收到就执行登出之类的
    "message": "操作成功",  		// 如果成功默认为操作成功，失败没有默认值
    "data": { "实际返回的数据" }   // 根据请求的dto不同而不同。如果请求失败了，则为空
  }
  ```

ps: 现在的response_dto不是完美的，等待后续更新

### 名规范

1. 文件命名：使用大驼峰命名法，例如 `NoteCard.qml`、`NetworkHandler.cpp`。
2. 页面文件：以 `View` 作为后缀名，放置于 `views` 文件夹中。
3. 自定义属性：使用小驼峰命名法，避免使用拼音。

### 导入组件

1. 使用loader         
2. import "../components/"             

> 需要导入某个路径内所有组件而不是某一特定组件，只能导入所有             
>
> - 如果使用了parent.width(或者parent.height)，确保parent的width/height已经设置，否则默认为0.虽然内容会不管父组件宽度而溢出(如果没设置换行), 但是之后想设置边框或者换行时就会出现bug        

## 🤝 **贡献**

欢迎贡献代码和建议！请按照以下步骤：

1. Fork 项目。
2. 创建功能分支：`git checkout -b feature-name`。
3. 提交修改：`git commit -m "Add feature-name"`。
4. 推送分支：`git push origin feature-name`。
5. 提交 PR 请求。

------

## 📞 **联系方式**

如有问题，请联系开发团队：

- [**Jerrylingj**](https://github.com/Jerrylingj)：lingj28@mail2.sysu.edu.cn
- **[water2027](https://github.com/water2027)**：linshy76@mail2.sysu.edu.cn
- **[math-zhuxy](https://github.com/math-zhuxy)**：zhuxy255@mail2.sysu.edu.cn
- **[YANGPuxyu](https://github.com/YANGPuxyu)**：yangpx26@mail2.sysu.edu.cn

------

<div align="center">   <p>&copy; 2024 AltAir. All Rights Reserved.</p> </div> 

