cmake_minimum_required(VERSION 3.11)

project(Bomberman2D
	VERSION 0.1
	DESCRIPTION "Multiplayer game 2D"
	LANGUAGES C)

# setting C11 standard for the whole project
set(CMAKE_C_STANDARD 11)

# adding global headers and structures
add_library(commons INTERFACE)
target_include_directories(commons INTERFACE common/include)

# adding subcatalogs that are separated for client and server
add_subdirectory(server)
add_subdirectory(client)
