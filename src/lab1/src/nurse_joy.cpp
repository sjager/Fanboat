#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <fanboat_ll/fanboatMotors.h>

fanboat_ll::fanboatMotors pubMsg;

const float BAND_WIDTH = 0.12;
const float DEFAULT_POWER = 0.12;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void inputCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  //ROS_INFO("I heard: %i and %i", msg->buttons[0], msg->buttons[1]);
  ROS_INFO("I heard: %5f, %5f", msg->axes[0], msg->axes[1]);

  //Store the original values of the controller
  float xAxis = msg->axes[0];
  float yAxis = msg->axes[1];

  //Stores the final values for the message
  float mot_r;
  float mot_l;

  //If the yAxis is within the band, set to minimum value
  float minYAxis = yAxis;
  if(yAxis > -BAND_WIDTH && yAxis < BAND_WIDTH){
    minYAxis = DEFAULT_POWER;
  }

  //If xAxis is negative, turn right
  if(xAxis < -BAND_WIDTH) {
    mot_r = minYAxis;
    mot_l = map(xAxis, 0, 1, DEFAULT_POWER, yAxis);
  //If xAxis is positive, turn left
  } else if (xAxis > BAND_WIDTH) {
    mot_l = minYAxis;
    mot_r = map(xAxis, -1, 0, yAxis, -DEFAULT_POWER);
  //If xAxis is within band, drive straight
  } else {
    mot_l = minYAxis;
    mot_r = minYAxis;
  }

  //Account for lack of symmetry between motors
  //mot_r /= 2;
  if(mot_r < BAND_WIDTH) mot_r = DEFAULT_POWER;
  if(mot_l < BAND_WIDTH) mot_l = DEFAULT_POWER;

  pubMsg.header = msg->header;
  pubMsg.left = mot_l;//((float) msg->buttons[0])/2.0;
  pubMsg.right = mot_r;//((float) msg->buttons[1])/2.0;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "nurse_joy_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<fanboat_ll::fanboatMotors>("/motors", 1000);
  ros::Subscriber sub = n.subscribe("/joy", 1000, inputCallback);

  ros::Rate loop_rate(8);

  pubMsg.left = .12;
  pubMsg.right = .12;

  while(ros::ok()) {
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
