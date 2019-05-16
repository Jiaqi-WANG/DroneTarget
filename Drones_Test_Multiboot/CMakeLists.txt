cmake_minimum_required(VERSION 2.8.3)
project(drone_test)

find_package(catkin REQUIRED 
  COMPONENTS
  roscpp
  rospy
  cv_bridge
  sensor_msgs
  image_transport
  std_msgs
)

find_package(Curses REQUIRED)

## Declare a catkin package
catkin_package(INCLUDE_DIRS include LIBRARIES ${PROJECT_NAME}_keyboard_controller)

include_directories(include ${catkin_INCLUDE_DIRS})

## Librairies
add_library(${PROJECT_NAME}_keyboard_controller src/keyboard_controller.cpp)
target_link_libraries(${PROJECT_NAME}_keyboard_controller ${roscpp_LIBRARIES})

## Executables
add_executable(${PROJECT_NAME}_main_controller src/main_controller.cpp )
target_link_libraries(${PROJECT_NAME}_main_controller ${catkin_LIBRARIES} ${CURSES_LIBRARIES} ${PROJECT_NAME}_keyboard_controller)

