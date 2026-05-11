set(SOURCES 
	src/game.c
	src/player.c
	src/server.c)

add_executable(server src/main.c)

# linking sources
target_sources(server PRIVATE ${SOURCES})

# linking headers
target_include_directories(server PRIVATE include)
target_link_libraries(server PRIVATE commons) # global
