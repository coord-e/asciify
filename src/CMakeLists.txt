cmake_minimum_required(VERSION 3.7)

add_executable(asciify_bin asciify.cpp)
set_target_properties(asciify_bin PROPERTIES OUTPUT_NAME asciify)
target_link_libraries(asciify_bin ${OpenCV_LIBS})

install(TARGETS asciify_bin RUNTIME DESTINATION bin)
