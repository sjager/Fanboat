#include <ros/ros.h>
#include <lab2/angle.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <math.h>

// Messages to be published by this node
lab2::angle triAngleMsg;
lab2::magnitude triMagnitudeMsg;

int main(int argc, char **argv) {

  ros::init(argc, argv, "triangle_node");
  ros::Nodehandle n;
  
  ros::Publisher angle = n.advertise<lab2::angle>("/joy_angle", 1000);
  
  ros::Publisher magnitude = n.advertise<lab2::magnitude>("/joy_magnitude",1000);
  
  ros::Rate loop_rate(8);
  
  pubAngleMsg.angle = 0;
  pubMagnitudeMsg.magnitude = 0;
  
  while(ros::ok()) {
  //finish the rest of the triangle stuff
  }
}
