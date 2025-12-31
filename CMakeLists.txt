cmake_minimum_required(VERSION 3.20)
project(TCP-Chat-App VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SQLite3)

include_directories(include)

add_executable(chat_server
    src/server_main.cpp
    src/Socket.cpp
    src/ChatServer.cpp
    src/DatabaseManager.cpp
)

target_link_libraries(chat_server PRIVATE SQLite::SQLite3)

add_executable(chat_client
    src/client_main.cpp
    src/Socket.cpp
)
target_link_libraries(chat_client pthread)


