cmake_minimum_required(VERSION 2.9)

project(Sockets)

include(CTest)

file(GLOB SOURCES src/Sockets/*.cpp)

include_directories(include)
add_library(${PROJECT_NAME} SHARED ${SOURCES} )

set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra")

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

file(GLOB HEADERS include/Sockets/*.hpp)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

add_subdirectory(tests)

add_executable(SocketDemo main.cpp)
target_link_libraries(SocketDemo ${PROJECT_NAME})