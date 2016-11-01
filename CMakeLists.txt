cmake_minimum_required(VERSION 3.5)
project(klyg)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/aash29/cpp/klyg/)

LINK_DIRECTORIES(/home/aash29/cpp/klyg/ /home/aash29/Software/Maple2015/bin.X86_64_LINUX/)

set(SOURCE_FILES l19maxwellSC.c contour.c vector.c cdeque.c dynarray.c)
add_executable(klyg ${SOURCE_FILES})

target_link_libraries(klyg m maplec maple hf)

