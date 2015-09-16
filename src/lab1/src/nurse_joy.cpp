#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <fanboat_ll/fanboatMotors.h>

fanboat_ll::fanboatMotors pubMsg;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void inputCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  //ROS_INFO("I heard: %i and %i", msg->buttons[0], msg->buttons[1]);
  ROS_INFO("I heard: %5f, %5f", msg->axes[0], msg->axes[1]);

  float xAxis = msg->axes[0];
  float yAxis = msg->axes[1];

  float mot_r;
  float mot_l;

  if(xAxis < -0.15) {
    mot_r = yAxis;
    mot_l = map(xAxis, 0, .8, 0, yAxis);
  } else if (xAxis > 0.15) {
    mot_l = yAxis;
    mot_r = map(xAxis, -0.8, 0, yAxis, 0);
  } else {
    mot_l = yAxis;
    mot_r = yAxis;
  }

  pubMsg.header = msg->header;
  pubMsg.left = mot_l;//((float) msg->buttons[0])/2.0;
  pubMsg.right = mot_r;//((float) msg->buttons[1])/2.0;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "nurse_joy_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<fanboat_ll::fanboatMotors>("/motors", 1000);
  ros::Subscriber sub = n.subscribe("/joy", 1000, inputCallback);

  ros::Rate loop_rate(10);

  while(ros::ok()) {
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
