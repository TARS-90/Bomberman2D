set(SOURCES 
	src/client.c)

add_executable(client src/main.c)

# linking sources
target_sources(client PRIVATE ${SOURCES})

# linking headers
target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE commons) # global
