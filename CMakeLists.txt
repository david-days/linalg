cmake_minimum_required(VERSION 3.6)
project(linalg C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(linalg
        eigen.c
        eigen.h
        errors.c
        errors.h
        linalg_obj.h
        linreg.c
        linreg.h
        linsolve.c
        linsolve.h
        main.c
        matrix.c
        matrix.h
        rand.c
        rand.h
        tests.c
        tests.h
        util.c
        util.h
        vector.c
        vector.h)
