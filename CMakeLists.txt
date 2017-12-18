cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)

project(asciify VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(asciify_bin main.cpp)
set_target_properties(asciify_bin PROPERTIES OUTPUT_NAME asciify)
target_link_libraries(asciify_bin ${OpenCV_LIBS})

install(TARGETS asciify_bin RUNTIME DESTINATION bin)
