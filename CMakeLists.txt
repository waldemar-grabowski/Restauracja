cmake_minimum_required(VERSION 3.10)
project(Restauracja)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find ncurses library (available on Unix/Linux/macOS)
find_package(Curses QUIET)

# Create main executable
add_executable(restauracja main.cpp)

if(CURSES_FOUND)
    # Use ncurses if available
    target_include_directories(restauracja PRIVATE ${CURSES_INCLUDE_DIRS})
    target_link_libraries(restauracja ${CURSES_LIBRARIES})
    message(STATUS "Building with ncurses support (${CURSES_LIBRARIES})")
else()
    # Fall back to console mode (Windows/no ncurses)
    target_compile_definitions(restauracja PRIVATE NO_NCURSES)
    message(STATUS "Building without ncurses - console mode only")
endif()

# Install target
install(TARGETS restauracja DESTINATION bin)
install(FILES menu.txt DESTINATION bin)
