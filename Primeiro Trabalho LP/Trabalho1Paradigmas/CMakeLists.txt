cmake_minimum_required(VERSION 3.31)
project(Trabalho1Paradigmas C)

set(CMAKE_C_STANDARD 23)

add_executable(Trabalho1Paradigmas main.c
        CdL_refcount.c
        CdL_refcount.h)
