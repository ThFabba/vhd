cmake_minimum_required(VERSION 2.8)
enable_testing()

add_library(systemcxx
    SystemCxx.cpp
    SystemFileCxx.cpp
    SystemLoggingCxx.cpp)

add_library(vhd
    VhdFile.cpp
    VhdUtils.cpp)

add_subdirectory(Tests)
