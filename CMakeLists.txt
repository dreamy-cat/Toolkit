cmake_minimum_required(VERSION 3.16)
project(Toolkit VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")
# set(CMAKE_LIBRARY_PATH "/usr/local/lib64")
#set(CMAKE_CXX_FLAGS "-std=c++17 -og -Wall -Wextra")
#target_link_libraries(${PROJECT_NAME} -lpthread)
#aux_source_directory(. SRC_LIST)

#include_directories(${inlclude})
#set(PROJECT_NAME main.cpp simple_client.cpp ${include}/range/v3/all.hpp)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
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

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Toolkit
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Qt6 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Toolkit SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Toolkit
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Toolkit PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Toolkit PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Toolkit
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Toolkit)
endif()
