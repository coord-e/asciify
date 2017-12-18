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
link_directories(/usr/local/lib64)

add_executable(edge_bin edge.cpp)
set_target_properties(edge_bin PROPERTIES OUTPUT_NAME edge)
target_link_libraries(edge_bin ${OpenCV_LIBS})

add_executable(ocr_bin ocr.cpp)
set_target_properties(ocr_bin PROPERTIES OUTPUT_NAME ocr)
target_link_libraries(ocr_bin ${OpenCV_LIBS} -lopencv_text)

add_executable(asciify_bin main.cpp)
set_target_properties(asciify_bin PROPERTIES OUTPUT_NAME asciify)
target_link_libraries(asciify_bin ${OpenCV_LIBS})
