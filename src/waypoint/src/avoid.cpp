#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab3/fanboatControl.h>

using lab3::fanboatControl;

fanboatControl controlMsg;

int main(int argc, char **argv) {
  ros::init(argc, argv, "avoid_state_node");

  ros::NodeHandle n;

  ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/state/avoid", 1000);
  
  ros::Rate loop_rate(8);

  while(ros::ok()) {

    controlPub.publish(controlMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
