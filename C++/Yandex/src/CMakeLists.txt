cmake_minimum_required(VERSION 3.5)

set(
	source_list
	"1_yellow_belt/week_1/week_1.h"
	"1_yellow_belt/week_1/week_1.cpp"
	"1_yellow_belt/week_2/week_2.h"
	"1_yellow_belt/week_2/week_2.cpp"
	"1_yellow_belt/week_3/week_3.h"
	"1_yellow_belt/week_3/week_3.cpp"
	"1_yellow_belt/week_4/week_4.h"
	"1_yellow_belt/week_4/week_4.cpp"
	"1_yellow_belt/week_5/week_5.h"
	"1_yellow_belt/week_5/week_5.cpp"
	"1_yellow_belt/week_6/condition_parser.h"
	"1_yellow_belt/week_6/condition_parser.cpp"
	"1_yellow_belt/week_6/condition_parser_test.cpp"
	"1_yellow_belt/week_6/database.h"
	"1_yellow_belt/week_6/database.cpp"
	"1_yellow_belt/week_6/database_test.cpp"
	"1_yellow_belt/week_6/date.h"
	"1_yellow_belt/week_6/date.cpp"
	"1_yellow_belt/week_6/node.h"
	"1_yellow_belt/week_6/node.cpp"
	"1_yellow_belt/week_6/test_runner.h"
	"1_yellow_belt/week_6/token.h"
	"1_yellow_belt/week_6/token.cpp"
	"2_red_belt/week_1/week_1.h"
	"2_red_belt/week_1/week_1.cpp"
	"2_red_belt/week_2/week_2.h"
	"2_red_belt/week_2/week_2.cpp"
	"2_red_belt/week_3/week_3.h"
	"2_red_belt/week_3/week_3.cpp"
	"2_red_belt/week_4/week_4.h"
	"2_red_belt/week_4/week_4.cpp"
	"2_red_belt/week_5/week_5.h"
	"2_red_belt/week_5/week_5.cpp"
	"unit_test/unit_test.h"
	"unit_test/unit_test.cpp"
	"time_logger/time_logger.h"
	"time_logger/time_logger.cpp"
	"main.cpp"
)

add_executable(${PROJECT_NAME} ${source_list})

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
