cmake_minimum_required(VERSION 2.8)

project(Sockets)

file(GLOB SOURCES src/*.cpp)

include_directories(include)

add_library(${PROJECT_NAME} SHARED ${SOURCES})


install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

file(GLOB HEADERS include/*.hpp)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})
