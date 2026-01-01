cmake_minimum_required(VERSION 3.20)
project(TCP-Chat-App VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)   

find_package(SQLite3)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(include)

add_executable(chat_server
    src/server_main.cpp
    src/Socket.cpp
    src/ChatServer.cpp
    src/DatabaseManager.cpp
)

target_link_libraries(chat_server PRIVATE SQLite::SQLite3)

add_executable(gui_client
    src/client_gui_main.cpp
    src/ChatWindow.cpp
    src/Socket.cpp
    include/ChatWindow.h
)
target_link_libraries(gui_client PRIVATE Qt5::Widgets pthread)


