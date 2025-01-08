cmake_minimum_required(VERSION 3.16)
project(Toolkit VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "-std=c++17 -og -Wall -Wextra")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Network)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(SOURCES
        DeviceTypeA.hpp
        DeviceTypeA.cpp
        main.cpp
        SimpleTCPClient.cpp
        SimpleTCPClient.hpp
        Toolkit.cpp
        Toolkit.hpp
        Toolkit.ui
        UDPDevice.hpp
        UDPDevice.cpp
        UDPServer.hpp
        UDPServer.cpp
)

qt_add_executable(Toolkit MANUAL_FINALIZATION ${SOURCES})
target_link_libraries(Toolkit PRIVATE Qt6::Widgets Qt6::Network)
set_target_properties(Toolkit PROPERTIES ${BUNDLE_ID_OPTION} WIN32_EXECUTABLE TRUE)
# Дополнительно, если требуется.
# qt_finalize_executable(Toolkit)
