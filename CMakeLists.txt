cmake_minimum_required(VERSION 3.5)

project(switch-qt-rpc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/discord-rpc.h
        src/discord-rpc.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(switch-qt-rpc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET switch-qt-rpc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(switch-qt-rpc SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(switch-qt-rpc
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(DISCORD_SDK_PATH vendor/discord)
set(CMAKE_DISCORD_SDK_SOURCES
    ${DISCORD_SDK_PATH}/src/achievement_manager.cpp
    ${DISCORD_SDK_PATH}/src/activity_manager.cpp
    ${DISCORD_SDK_PATH}/src/application_manager.cpp
    ${DISCORD_SDK_PATH}/src/core.cpp
    ${DISCORD_SDK_PATH}/src/image_manager.cpp
    ${DISCORD_SDK_PATH}/src/lobby_manager.cpp
    ${DISCORD_SDK_PATH}/src/network_manager.cpp
    ${DISCORD_SDK_PATH}/src/overlay_manager.cpp
    ${DISCORD_SDK_PATH}/src/relationship_manager.cpp
    ${DISCORD_SDK_PATH}/src/storage_manager.cpp
    ${DISCORD_SDK_PATH}/src/store_manager.cpp
    ${DISCORD_SDK_PATH}/src/types.cpp
    ${DISCORD_SDK_PATH}/src/user_manager.cpp
    ${DISCORD_SDK_PATH}/src/voice_manager.cpp
    ${DISCORD_SDK_PATH}/src/achievement_manager.h
    ${DISCORD_SDK_PATH}/src/activity_manager.h
    ${DISCORD_SDK_PATH}/src/application_manager.h
    ${DISCORD_SDK_PATH}/src/core.h
    ${DISCORD_SDK_PATH}/src/image_manager.h
    ${DISCORD_SDK_PATH}/src/lobby_manager.h
    ${DISCORD_SDK_PATH}/src/network_manager.h
    ${DISCORD_SDK_PATH}/src/overlay_manager.h
    ${DISCORD_SDK_PATH}/src/relationship_manager.h
    ${DISCORD_SDK_PATH}/src/storage_manager.h
    ${DISCORD_SDK_PATH}/src/store_manager.h
    ${DISCORD_SDK_PATH}/src/types.h
    ${DISCORD_SDK_PATH}/src/user_manager.h
    ${DISCORD_SDK_PATH}/src/voice_manager.h
    ${DISCORD_SDK_PATH}/src/discord.h
    ${DISCORD_SDK_PATH}/src/event.h
    ${DISCORD_SDK_PATH}/src/ffi.h
)


add_library(discord_game_sdk STATIC ${CMAKE_DISCORD_SDK_SOURCES})


target_include_directories(switch-qt-rpc PUBLIC ${DISCORD_SDK_PATH}/src)

target_link_libraries(switch-qt-rpc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

get_filename_component(DISCORD_SDK_LIB_ABS_PATH ${DISCORD_SDK_PATH}/lib/x86_64/discord_game_sdk.dll.lib ABSOLUTE)

target_link_libraries(switch-qt-rpc PUBLIC discord_game_sdk ${DISCORD_SDK_LIB_ABS_PATH})
install(FILES ${DISCORD_SDK_PATH}/lib/x86_64/discord_game_sdk.dll.lib DESTINATION lib/)

set_target_properties(switch-qt-rpc PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(switch-qt-rpc)
endif()
