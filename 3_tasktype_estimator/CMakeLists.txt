cmake_minimum_required(VERSION 3.22.1)
project(experiment)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES scripts)
include_directories(include)
add_executable(main
        main.cpp
        constants.cpp
        config.cpp
        estimation/measure.cpp
        tools.cpp
        taskmapper/taskmapperOneToOne.cpp
        taskmapper/taskmapperLog.cpp
        taskmapper/taskmapperOneToMany.cpp
        estimation/estimator.cpp
        taskmapper/taskmapperDiffsearch.cpp
        taskmapper/taskmapperPairsearch.cpp
        include/taskmapperDiffsearch.h
)