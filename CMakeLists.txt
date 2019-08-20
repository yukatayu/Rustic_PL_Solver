cmake_minimum_required(VERSION 2.6)
project(cpp_rustic_sat CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(./Rustic_SAT_Solver)

add_executable(smt_solver main.cpp)
target_link_libraries(smt_solver rustic_sat)

