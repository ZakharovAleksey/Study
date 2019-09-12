# Author: Zakharov Aleksey
cmake_minimum_required(VERSION 3.5)

# Add all *.cpp & *.h files to project
file(GLOB_RECURSE source_list 
	LIST_DIRECTORIES false
	"0_white_belt/*.cpp"
	"0_white_belt/*.h"
	"1_yellow_belt/*.cpp"
	"1_yellow_belt/*.h"
	"2_red_belt/*.cpp"
	"2_red_belt/*.h"
	"3_brown_belt/*.cpp"
	"3_brown_belt/*.h"	
	"main.cpp"
)

# Create an executable projecrt
add_executable(${PROJECT_NAME} ${source_list})

# Add libraries we want to use
target_link_libraries(${PROJECT_NAME} PUBLIC
	logging
	unit_testing
)

# This allows to usage of #include<external_library>
target_include_directories(${PROJECT_NAME} PUBLIC
	../time_logger
	../unit_test
)

foreach(_source IN ITEMS ${source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()