cmake_minimum_required(VERSION 3.16)

project(FlightManagementSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Network)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appuntitled7
    main.cpp
    api/networkhandler.h
    api/networkhandler.cpp
)

qt_add_qml_module(appuntitled7
    URI untitled7
    VERSION 1.0
    QML_FILES
        Main.qml
        components/NoteList.qml
        components/NoteCard.qml
        views/HomeView.qml
        views/FlightInfoView.qml
        views/OrdersView.qml
        views/ProfileView.qml
        views/NoteDetailView.qml
        components/FlightInfoCard.qml
        views/login.qml
        static/sites.js
    RESOURCES
        android/AndroidManifest.xml
        android/res/xml/network_security_config.xml
        figure/login-flight.png
    SOURCES
        api/networkhandler.h
        api/networkhandler.cpp

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appuntitled7 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled7
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appuntitled7
    PRIVATE Qt6::Quick Qt6::Network
)

set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android")

include(GNUInstallDirs)
install(TARGETS appuntitled7
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
