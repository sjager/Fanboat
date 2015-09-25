#include <ros/ros.h>
#include <fanboat_ll/fanboatMotors.h>

fanboat_ll::fanboatMotors pubMsg;

const float BAND_WIDTH = 0.12;
const float DEFAULT_POWER = 0.12;

int main(int argc, char **argv) {
  ros::init(argc, argv, "forward_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<fanboat_ll::fanboatMotors>("/motors", 1000);
  
  double parameter, m, b;
  n.getParam("forward_input", parameter);
  n.getParam("m", m);
  n.getParam("b", b);
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

  pubMsg.left = .48;
  pubMsg.right = .48;
  iterator=0;

  parameter = (parameter - b)/m;

  while(ros::ok() && iterator < parameter * 13) {
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
    iterator++;
  }

  return 0;
}
