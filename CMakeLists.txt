cmake_minimum_required(VERSION 3.6)
project(wav2mp3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address")
endif(CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(LAME REQUIRED)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	find_package(Pthreads4W REQUIRED)
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	SET(STD_FS_LIBRARIES stdc++fs)
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

include_directories(${LAME_INCLUDE_DIRS} ${PTHREADS4W_INCLUDE_DIRS})

add_executable(wav2mp3 main.cpp wavefile.cpp jobqueue.cpp encodejob.cpp)

target_link_libraries(wav2mp3 ${LAME_LIBRARIES} Threads::Threads ${STD_FS_LIBRARIES} ${PTHREADS4W_LIBRARIES})

install(TARGETS wav2mp3 RUNTIME DESTINATION bin)
