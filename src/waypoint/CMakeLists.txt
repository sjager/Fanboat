cmake_minimum_required(VERSION 2.8.3)
project(waypoint)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

#add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES waypoint
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

#add_executable(graphics_arbitrator_node src/graphics_arbitrator.cpp)
#add_dependencies(graphics_arbitrator_node lab3_generate_messages_cpp)
#target_link_libraries(graphics_arbitrator_node ${catkin_LIBRARIES})
