add_library(commons STATIC
	src/queue.c
	src/list.c
)

target_include_directories(commons PUBLIC include)
