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

option(FORMAT_BEFORE_BUILD
  "If the command clang-format is avilable, format source files before each build.\
Turn this off if the build time is too slow."
  ON)
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
  message(STATUS "Enable Clang-Format")
  file(GLOB_RECURSE files src/*.cpp)
  add_custom_target(
    format
    COMMAND clang-format -i -style=file ${files}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
  add_custom_target(format_build)
  if(FORMAT_BEFORE_BUILD)
    message(STATUS "Enable build-time formatting")
    add_dependencies(format_build format)
  endif()
else()
    message(WARNING "Could not find clang-format")
endif()

add_subdirectory(src)
