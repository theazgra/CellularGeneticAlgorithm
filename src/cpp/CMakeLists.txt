cmake_minimum_required(VERSION 3.2.0)
project(cellular-ga VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(cellular-ga main.cpp)


find_package (Threads)
target_link_libraries (cellular-ga ${CMAKE_THREAD_LIBS_INIT})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)