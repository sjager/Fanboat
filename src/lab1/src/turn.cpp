#include <ros/ros.h>
#include <fanboat_ll/fanboatMotors.h>

fanboat_ll::fanboatMotors pubMsg;

const float BAND_WIDTH = 0.12;
const float DEFAULT_POWER = 0.12;

int main(int argc, char **argv) {
  ros::init(argc, argv, "turn_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<fanboat_ll::fanboatMotors>("/motors", 1000);
  
  double parameter;
  n.getParam("degree_input", parameter);
  //ROS_INFO("Got parameter: %i", parameter);

  ros::Rate loop_rate(13);
  
  pubMsg.left = .12;
  pubMsg.right = .12;

  int iterator=0;
  while(ros::ok() && iterator < 39) {
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
    iterator++;
  }

  ROS_INFO("%f", parameter);
  if(parameter <= 180.0){
    pubMsg.left = .47;
    pubMsg.right = .12;
  } else {
    pubMsg.right = .47;
    pubMsg.left = .12;
    parameter = (parameter - 360.0) * -1;
  }
  iterator=0;
  ROS_INFO("%f", parameter);
  while(ros::ok() && iterator < parameter/2.0 * .13) {
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
    iterator++;
  }

  return 0;
}
