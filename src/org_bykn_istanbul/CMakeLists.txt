cmake_minimum_required (VERSION 2.8)
project (istanbul)

add_executable(hello hello.cpp option.hpp)

set_target_properties(hello
	PROPERTIES COMPILE_FLAGS "--std=c++0x")
