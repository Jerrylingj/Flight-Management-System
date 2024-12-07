cmake_minimum_required(VERSION 3.16)
project(QtHttpServer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Network
    HttpServer
    Sql
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
qt_add_resources(PROJECT_SOURCES
    config/database_config.qrc
)
add_executable(QtHttpServer
    ${PROJECT_SOURCES}
    main.cpp
    database/databasemanager.cpp
    database/databasemanager.h
    config/database_config.json
    config/database_config.qrc
    config/database_config.template.json
    dto/response_dto.h dto/response_dto.cpp
    dto/flight_info_dto.h dto/flight_info_dto.cpp
    api/login/login.h api/login/login.cpp
    api/register/register.h api/register/register.cpp

    api/flight/FlightAPI.h api/flight/FlightAPI.cpp
    dto/login_dto.h dto/login_dto.cpp

)
target_link_libraries(QtHttpServer PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::HttpServer
    Qt6::Sql
)
set_target_properties(QtHttpServer PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
