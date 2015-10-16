#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <lab2/mux_control.h>

#define A_INPUT 1
#define B_INPUT 2
#define X_INPUT 3
#define Y_INPUT 4

#define INVALID_INPUT 0

lab2::mux_control muxControlPubMsg;

void inputCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  
  int input = INVALID_INPUT;
  
  int aDown = msg->buttons[0];
  int bDown = msg->buttons[1];
  int xDown = msg->buttons[2];
  int yDown = msg->buttons[3];
  
  if(aDown == 1) {
    input = A_INPUT;
  } else if(bDown == 1) {
    input = B_INPUT;
  } else if(xDown == 1) {
    input = X_INPUT;
  } else if(yDown == 1) {
    input = Y_INPUT;
  }
  
  
  if(aDown || bDown || xDown || yDown) {
    muxControlPubMsg.state = input;
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "mux_control_node");
  
  ros::NodeHandle n;
  
  ros::Publisher muxControlPub = n.advertise<lab2::mux_control>("/mux_control", 1000);
  
  ros::Subscriber sub = n.subscribe("/joy", 1000, inputCallback);
  
  ros::Rate loop_rate(8);
  
  muxControlPubMsg.state = INVALID_INPUT; 
  muxControlPub.publish(muxControlPubMsg);
  
  while(ros::ok()) {
  
    muxControlPub.publish(muxControlPubMsg);
    
    ros::spinOnce();
    loop_rate.sleep();
  }
  
  return 0; 
}
