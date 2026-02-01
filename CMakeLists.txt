cmake_minimum_required(VERSION 3.16)
project(Restauracja)

set(CMAKE_CXX_STANDARD 11)

add_executable(restauracja main.cpp)

if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

add_custom_command(TARGET restauracja POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/menu.txt"
        "$<TARGET_FILE_DIR:restauracja>/menu.txt"
)

install(TARGETS restauracja DESTINATION bin)
install(FILES menu.txt DESTINATION bin)


