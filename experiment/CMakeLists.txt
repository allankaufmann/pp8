cmake_minimum_required(VERSION 3.22.1)
project(experiment)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES testRaplRead.sh readEnergy_UJ.sh scripts)

add_executable(experiment main.cpp)
