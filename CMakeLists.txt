cmake_minimum_required(VERSION 3.15)
project(supagrep VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS SvgWidgets)
qt_standard_project_setup()

add_executable(supagrep
	srcqt/mainqt.h
	srcqt/mainqt.cc
	srcqt/searchwinqt.h
	srcqt/searchwinqt.cc )

target_link_libraries(supagrep PRIVATE Qt6::Core)
target_link_libraries(supagrep PRIVATE Qt6::Widgets)
target_link_libraries(supagrep PRIVATE Qt6::SvgWidgets)
