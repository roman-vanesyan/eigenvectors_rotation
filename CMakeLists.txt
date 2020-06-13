cmake_minimum_required(VERSION 3.15)
project(evc_26_10 C)

set(CMAKE_C_STANDARD 99)

add_executable(evc_26_10 task_26_10.h sim_26_10.c main_26_10.c evc_26_10.c)
target_link_libraries(evc_26_10 m)
