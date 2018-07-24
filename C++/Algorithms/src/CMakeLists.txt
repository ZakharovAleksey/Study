cmake_minimum_required(VERSION 3.5)

set(
	source_list
	"sorting/sorting.h"
	"sorting/sorting.cpp"
	"data_structures/data_structures.h"
	"data_structures/data_structures.cpp"
	"data_structures/bloom_filter.h"
	"practice/practice.h"
	"practice/practice.cpp"
	"unit_test/unit_test.h"
	"unit_test/unit_test.cpp"	
	"main.cpp"
)

add_executable(${PROJECT_NAME} ${source_list})

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
