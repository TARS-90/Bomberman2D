cmake_minimum_required(VERSION 3.11)

project(Bomberman2D
	VERSION 0.1
	DESCRIPTION "Multiplayer game 2D"
	LANGUAGES C)

# setting C11 standard for the whole project
set(CMAKE_C_STANDARD 11)


add_subdirectory(commons) # global
add_subdirectory(server)
add_subdirectory(client)
