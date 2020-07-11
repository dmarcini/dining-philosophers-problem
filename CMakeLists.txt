cmake_minimum_required(VERSION 3.10)

project(dining-philosophers-problem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -lncurses")

find_package(Threads)
find_package(Curses)

include_directories(include)

set(SOURCES src/main.cpp
	    src/fork.cpp
	    src/philosopher.cpp
	    src/table.cpp
	    src/visualisation.cpp)

add_executable(dining-philosophers-problem ${SOURCES})

target_link_libraries(dining-philosophers-problem Threads::Threads curses)
