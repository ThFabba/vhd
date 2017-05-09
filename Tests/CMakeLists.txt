

include_directories(..)

add_executable(systemfile_test
    TestSystemFile.cpp
    Test.cpp)
target_link_libraries(systemfile_test systemcxx)
add_test(NAME systemfile_test
    COMMAND systemfile_test
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(vhdutils_test
    TestVhdUtils.cpp
    Test.cpp)
target_link_libraries(vhdutils_test systemcxx vhd)
add_test(NAME vhdutils_test
    COMMAND vhdutils_test
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(vhdfile_test
    TestVhdFile.cpp
    Test.cpp)
target_link_libraries(vhdfile_test systemcxx vhd)
add_test(NAME vhdfile_test
    COMMAND vhdfile_test
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
