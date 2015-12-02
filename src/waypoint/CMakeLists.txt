cmake_minimum_required(VERSION 2.8.3)
project(waypoint)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

add_message_files(
  FILES
  arduinoAngle.msg
  fanboatInfo.msg
 )

generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES waypoint
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)



include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(arduino_sweep_node src/arduino_sweep.cpp)
add_dependencies(arduino_sweep_node waypoint_generate_messages_cpp)
target_link_libraries(arduino_sweep_node ${catkin_LIBRARIES})
