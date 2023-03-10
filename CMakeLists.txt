cmake_minimum_required(VERSION 3.5)
project(thread_syncronization)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(main src/main.cpp)

target_link_libraries(main
  ${catkin_LIBRARIES}
)
