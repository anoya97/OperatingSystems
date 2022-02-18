cmake_minimum_required(VERSION 3.20)
project(p0 C)

set(CMAKE_C_STANDARD 99)

add_executable(p0 p1.c hist_list.h hist_list.c types.h)
