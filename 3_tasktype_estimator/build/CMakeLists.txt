cmake_minimum_required(VERSION 3.22.1)
project(experiment)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES scripts)

add_executable(
        config.cpp
        main.cpp
        estimation/measure.cpp
        constants.h
        tools.cpp
        taskmapper/taskmapperOneToOne.cpp
        taskmapper/taskmapperLog.cpp
        taskmapper/taskmapperOneToMany.cpp
        estimation/estimator.cpp
        estimation/estimator.cpp
        taskmapper/taskmapperDiffsearch.cpp
        taskmapper/taskmapperPairsearch.cpp
)

