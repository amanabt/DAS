#Cmake setup
cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)
set(CMAKE_CXX_COMPILER ccache g++)
project(busybox)
set(CMAKE_VERBOSE_MAKEFILE ON) #Verbose

#Include the header directory
#Adding sources
set(SOURCES
	busybox.cxx)
#Optimization flag
set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++11 -lpthread")

#Compile

#set_target_properties(busybox PROPERTIES COMPILE_FLAGS "-O3 -Wall -std=c++11 -lpthread")
add_executable(busybox ${SOURCES})
