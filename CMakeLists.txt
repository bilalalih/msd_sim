cmake_minimum_required(VERSION 3.20)
project(msd_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(msd_lib
  src/model.cpp
  src/input.cpp
  src/integrators.cpp
  src/csv.cpp
  src/metrics.cpp
)

target_include_directories(msd_lib PUBLIC include)

add_executable(msd_sim apps/msd_main.cpp)
target_link_libraries(msd_sim PRIVATE msd_lib)
