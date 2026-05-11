cmake_minimum_required(VERSION 3.10)


project(server 
	VERSION 0.1
	DESCRIPTION "Server that will be used for game"
	LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set(SOURCE )
add_executable(main 
	src/main.c
	src/game.c
	src/playerc)
include_directories(./include/ ../common/include/)
