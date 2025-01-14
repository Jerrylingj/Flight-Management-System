# 🌍 AltAir - Backend Repository

[English Version](README.md) | [简体中文](README-zh.md)



<div align="center">
  <img src="./favicon.jpg" alt="Altair Backend Logo" style="border-radius: 8px;"/>
</div>


------

## 🚀 **Project Overview**

**AltAir** is the backend module of a full-stack flight management system developed by the **[Terminal Terrace Team](https://github.com/Terminal-Terrace)**  . Built using **C++** and **MySQL**, the backend is designed to provide efficient, secure, and reliable API services for both user and admin operations, including:

- **User Management**
- **Flight Management**
- **Order Processing**
- **Data Interaction and Security**

------

## 🖂 **Project Structure**

```plaintext
├── backend/                 # Core backend module
│   ├── aichat/              # AI Chat module for intelligent customer service
│   ├── api/                 # API routes and logic, calls database functions to implement features
│   ├── config/              # Global configuration files (e.g., database settings)
│   ├── database/            # Database operations (CRUD)
│   ├── doc/                 # Documentation, including API specifications and data structures
│   ├── dto/                 # Data Transfer Objects (DTO) for uniform data exchange
│   ├── util/                # Utility functions (e.g., encryption, network requests)
│   ├── main.cpp             # Backend program entry point, initializes services and routes
│   └── CMakeLists.txt       # Build configuration using CMake
├── fetchFlights/            # Module for dynamic flight data scraping
├── frontend/                # Deprecated mobile front-end code
├── microBackend/            # Micro-backend module for email registration and login
└── README.md                # Project documentation
```

------

## ⚙️ **Development Environment**

| Requirement       | Version         |
| ----------------- | --------------- |
| **C++ Version**   | C++17           |
| **MySQL Version** | 8.0+            |
| **Build Tool**    | CMake 3.15      |
| **OS**            | Windows / Linux |

------

## 🛠️ **Getting Started**

### 1. Clone the Repository

```bash
git clone git@github.com:Jerrylingj/Flight-Management-System.git
```

------

### 2. Configure MySQL and ODBC

1. Install and configure **MySQL** database.

2. Set up the ODBC driver for database connections.

3. Create a `database_config.json` file in both `backend/config/` and `fetchFlights/` directories using the provided template `database_config.template.json`:

   ```json
   {
       "hostname": "127.0.0.1",
       "port": 3306,
       "database": "your_database",
       "username": "your_username",
       "password": "your_password"
   }
   ```

------

### 3. Start the Flight Data Scraping Module

1. Run the `fetchFlights/get_flights.exe` file.

2. Insert scraped data into the database by running:

   ```bash
   node main.js
   ```

------

### 4. Configure Email Registration and Login

1. Create an `smtp.json` file in the `microBackend/` directory using the provided `smtp.template.json` template:

   ```json
   {
       "host": "smtp.qq.com",
       "port": 465,
       "secure": true,
       "user": "your-email@qq.com",
       "pass": "your-smtp-auth-code"
   }
   ```

2. Start the service:

   ```bash
   node app.js
   ```

3. Successful configuration will display:

   ```bash
   Server is running on port 3000
   Reading SMTP configuration...
   Server is ready to send emails
   ```

------

### 5. Configure AI customer service



------

## ✨ **Features**

### Core Features

- **User Management**: User registration, login, and account management.
- **Flight Management**: Admin can add, update, or delete flight information.
- **Order Management**: Users can create, pay for, and query orders.
- **AI Customer Service**: Integrated intelligent customer support.

### Highlights

- **Modular Design**: Clear layered architecture, easy to maintain and extend.
- **Data Security**: AES256 encryption for user data and SSL/TLS for payment interactions.
- **Scalability**: Supports distributed database architecture and optimized for high performance.

------

## 🛠️ **Notes**

### Data Transfer Objects (DTO)

- **Definition**: All request and response data follow DTO specifications, managed in the `dto/` directory.

- **Standard Response Format**:

  ```json
  {
    "code": 200,
    "message": "Operation successful",
    "data": { "actual_returned_data" }
  }
  ```

### Naming Conventions

1. **File Naming**: Use PascalCase for file names, e.g., `NoteCard.qml`, `NetworkHandler.cpp`.
2. **View Files**: Suffix view files with `View` and place them in the `views` folder, e.g., `HomeView.qml`.
3. **Custom Attributes**: Use camelCase for custom properties and avoid non-English names.

------

## 🤝 **Contributing**

Contributions are welcome! Follow these steps:

1. Fork the repository.
2. Create a feature branch: `git checkout -b feature-name`.
3. Commit your changes: `git commit -m "Add feature-name"`.
4. Push your branch: `git push origin feature-name`.
5. Open a pull request.

------

## 📞 **Contact Us**

For any questions or suggestions, please contact the team:

- [**Jerrylingj**](https://github.com/Jerrylingj): [lingj28@mail2.sysu.edu.cn](mailto:lingj28@mail2.sysu.edu.cn)
- [**water2027**](https://github.com/water2027): [linshy76@mail2.sysu.edu.cn](mailto:linshy76@mail2.sysu.edu.cn)
- [**math-zhuxy**](https://github.com/math-zhuxy): [zhuxy255@mail2.sysu.edu.cn](mailto:zhuxy255@mail2.sysu.edu.cn)
- [**YANGPuxyu**](https://github.com/YANGPuxyu): [yangpx26@mail2.sysu.edu.cn](mailto:yangpx26@mail2.sysu.edu.cn)

------

<div align="center">   <p>&copy; 2024 AltAir. All Rights Reserved.</p> </div> 
