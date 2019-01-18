cmake_minimum_required(VERSION 2.8.3)
project(magnetic_valve_switch)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  ur_msgs
  message_generation
)


## Generate services in the 'srv' folder
add_service_files(
  FILES
  switcher.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs  ur_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES magnetic_valve_switch
 CATKIN_DEPENDS roscpp std_msgs ur_msgs message_generation
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(gripper_switch_server src/gripper_switch_server.cpp)
target_link_libraries(gripper_switch_server ${catkin_LIBRARIES})