cmake_minimum_required(VERSION 2.8)

project(PERSISTENT_SET)

include_directories(${PERSISTENT_SET_SOURCE_DIR})

add_executable(PERSIST_SET shared_ptr.h
			   linked_ptr.h
		           persistent_set.h
		           test.cpp)


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()
