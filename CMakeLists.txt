cmake_minimum_required(VERSION 3.16)

project(tedHttpApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS
    Core
    Quick
    Network
    HttpServer
)

qt_standard_project_setup()

qt_add_executable(apptedHttpApp
    main.cpp
    src/TedManager.h src/TedManager.cpp
    src/network/client/RequestManager.h src/network/client/RequestManager.cpp
    src/network/connection/ConnectionManager.h src/network/connection/ConnectionManager.cpp
    src/network/connection/ConnectedDeviceModel.h src/network/connection/ConnectedDeviceModel.cpp
    src/network/server/CommandServer.h src/network/server/CommandServer.cpp
    src/utils/Log.h src/utils/Log.cpp
)

qt_add_qml_module(apptedHttpApp
    URI tedHttpApp
    VERSION 1.0
    QML_FILES Main.qml
)

include_directories(
    src
    src/network/connection
    src/network/client
    src/network/client/http
    src/network/server
    src/utils
)

set_target_properties(apptedHttpApp PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptedHttpApp PRIVATE
    Qt6::Quick
    Qt6::Network
    Qt6::HttpServer
)

install(TARGETS apptedHttpApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
