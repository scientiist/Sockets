cmake_minimum_required(VERSION 2.8)

project(Sockets)

file(GLOB SOURCES src/*.cpp)

include_directories(include)
add_library(${PROJECT_NAME} SHARED ${SOURCES} include/HttpClient.hpp)

set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra")

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

file(GLOB HEADERS include/*.hpp)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

add_executable(SocketTest main.cpp)
target_link_libraries(SocketTest ${PROJECT_NAME})