find_package(SDL3 REQUIRED) 

set(SOURCES 
	src/sdl_engine.c
	src/client.c)

add_executable(client src/main.c)

# linking sources
target_sources(client PRIVATE ${SOURCES})

# linking headers
target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE commons) # global

# linking SDL3 library
target_link_libraries(client PRIVATE SDL3::SDL3)
