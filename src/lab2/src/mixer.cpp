#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <math.h>
#include <algorithm>

fanboat_ll::fanboatMotors angleMsg;
lab2::magnitude magMsg;

fanboat_ll::fanboatMotors pubMsg;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
    if(magMsg.magnitude >= .12)
    {
      mot_r = angleMsg.right + magMsg.magnitude;//map(angleMsg.right, 0.12, 0.95, 0.12, magMsg.magnitude);
      mot_l = angleMsg.left + magMsg.magnitude;//map(angleMsg.left, 0.12, 0.95, 0.12, magMsg.magnitude);
      if(mot_r < .12) mot_r = .12;
      if(mot_l < .12) mot_l = .12;
      if(mot_r > .95) mot_r = .95;
      if(mot_l > .95) mot_l = .95;
      pubMsg.right = mot_r;
      pubMsg.left = mot_l;
    }
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
