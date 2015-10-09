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

  

  float mot_r;
  float mot_l;

  while(ros::ok()) {
    
    float right = angleMsg.right;
    float left = angleMsg.left;
    
    float biggerVal = left;
    
    if(right <= left) {
      biggerVal = right;
    }
    
    float range = FAN_MAX - biggerVal;
    
    float constant = range * magMsg.magnitude;
    
    mot_r += constant;
    mot_l += constant;
    
    //these checks aren't technically needed, but just in case.
    if(mot_r > FAN_MAX) {
      mot_r = FAN_MAX;
    }
    
    if(mot_l > FAN_MAX) {
      mot_l = FAN_MAX;
    }
    
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
