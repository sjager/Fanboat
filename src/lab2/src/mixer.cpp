#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <math.h>
#include <algorithm>

#define FAN_MIN .12
#define FAN_MAX .9

fanboat_ll::fanboatMotors angleMsg;
lab2::magnitude magMsg;

fanboat_ll::fanboatMotors pubMsg;

void magInputCallback(const lab2::magnitude::ConstPtr& msg) {
  magMsg = *msg;
}

void angleInputCallback(const fanboat_ll::fanboatMotors::ConstPtr& msg){
  angleMsg = *msg;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "mixer_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<fanboat_ll::fanboatMotors>("/motors", 1000);
  ros::Subscriber anglesub = n.subscribe("/angle_motors", 1000, angleInputCallback);
  ros::Subscriber magsub = n.subscribe("/target_magnitude", 1000, magInputCallback);
  ros::Rate loop_rate(8);
  
  
  pubMsg.right = .12;
  pubMsg.left = .12;

  while(ros::ok()) {
    float right = angleMsg.right;
    float left = angleMsg.left;
    
    //right = .9;
    //left = .1;


    //ROS_INFO("RA: %f, LA: %f", right, left);

    float biggerVal = left;
    
    if(right >= left) {
      biggerVal = right;
    }
    
    float range = FAN_MAX - biggerVal;
    
    //ROS_INFO("Range: %f", range);


    float magn = magMsg.magnitude;

    //magn = .5;

    float constant = range * magn;
    
    right += constant;
    left += constant;
    
    //ROS_INFO("Const: %f", constant);

    //these checks aren't technically needed, but just in case.
    if(right > FAN_MAX) {
      right = FAN_MAX;
    }
    
    if(left > FAN_MAX) {
      left = FAN_MAX;
    }
    
    if(right < .12) right = .12;
    if(left < .12) left = .12;


    //ROS_INFO("RS: %f, LS: %f", right, left);
    ROS_INFO("RA: %f, LA: %f, RN: %f, C: %f, RS: %f LS: %f", angleMsg.right, angleMsg.left, range, constant, right, left);    


    pubMsg.right = right;
    pubMsg.left = left;

    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
