#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>

lab2::angle anglePubMsg;
lab2::magnitude magPubMsg;

const float BAND_WIDTH = 0.12;
const float DEFAULT_POWER = 0.12;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void inputCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  anglePubMsg.header = msg->header;
  anglePubMsg.angle = .12;

  magPubMsg.header = msg->header;
  magPubMsg.magnitude = .34;
  //anglePubMsg.left = mot_l;//((float) msg->buttons[0])/2.0;
  //anglePubMsg.right = mot_r;//((float) msg->buttons[1])/2.0;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "joy_splitter_node");

  ros::NodeHandle n;

  ros::Publisher anglePub = n.advertise<lab2::angle>("/target_angle", 1000);
  ros::Publisher magnitudePub = n.advertise<lab2::magnitude>("/target_magnitude", 1000);
  ros::Subscriber sub = n.subscribe("/joy", 1000, inputCallback);

  ros::Rate loop_rate(13);

  while(ros::ok()) {
    anglePub.publish(anglePubMsg);
    magnitudePub.publish(magPubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
