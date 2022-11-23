cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
if (CMAKE_CXX_FLAGS MATCHES .*_WINDOWS.*)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zc:__cplusplus")
endif()

project(chronos VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(chronos src/chronos.cpp)
set_property(TARGET chronos PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
