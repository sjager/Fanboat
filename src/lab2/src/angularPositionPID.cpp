#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <fanboat_ll/fanboatMotors.h>

fanboat_ll::fanboatMotors pubMsg;

const float BAND_WIDTH = 0.12;
const float DEFAULT_POWER = 0.12;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void inputCallback(const fanboat_ll::fanboatMotors::ConstPtr& msg) {
  ROS_INFO("I heard: a message");
  pubMsg = *msg;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "nurse_joy_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<fanboat_ll::fanboatMotors>("/motors", 1000);
  ros::Subscriber sub = n.subscribe("/target_angle", 1000, inputCallback);

  ros::Rate loop_rate(8);

  while(ros::ok()) {
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
