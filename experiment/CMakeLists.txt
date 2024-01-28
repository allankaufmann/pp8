cmake_minimum_required(VERSION 3.22.1)
project(experiment)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES scripts)

add_executable(experiment
        config.cpp
        main.cpp
        measure.cpp
        search.cpp
        tools.cpp
        constants.h

)
