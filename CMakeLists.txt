cmake_minimum_required(VERSION 3.5)
project(MenuTree LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

include_directories(include/)
file(GLOB_RECURSE src *.cpp *.c)
file(GLOB_RECURSE headers *.h *.hpp)

add_executable(${CMAKE_PROJECT_NAME}
    ${headers}
    ${src}
)
target_link_libraries(${CMAKE_PROJECT_NAME}
    Qt5::Core
)
