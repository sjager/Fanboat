#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <math.h>

#define WAIT_TIME 5
#define FWD_MAGNITUDE .7

// Messages to be published by this node
lab2::angle pubAngleMsg;
lab2::magnitude pubMagnitudeMsg;

int switcher = 1;
float angleVal = 0.0;

void timerCallback(const ros::TimerEvent& event)
{
  if(switcher >= 4) {
    
    ROS_INFO("forward, switcher: %i", switcher);
    if(switcher >= 5){
      pubMagnitudeMsg.magnitude = 0;
      pubMagnitudeMsg.ignoreAngle = true;
      switcher = 1;
    } else {
      pubMagnitudeMsg.magnitude = FWD_MAGNITUDE;
      pubMagnitudeMsg.ignoreAngle = true;
      switcher++;
    }
  } else {
    if(switcher == 1){
      angleVal += 120;
    }
    angleVal = fmod(angleVal,360.0);
    pubAngleMsg.angle = angleVal;
    pubMagnitudeMsg.magnitude = 0.0;
    pubMagnitudeMsg.ignoreAngle = false;
    ROS_INFO("turn: %f", pubAngleMsg.angle);
   
    switcher++;
  }
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "triangle_node");
  ros::NodeHandle n;
  
  ros::Timer timer = n.createTimer(ros::Duration(2), timerCallback);

  //These publishers individually publish angle and magnitude info
  ros::Publisher anglePub = n.advertise<lab2::angle>("/tri_angle", 1000);  
  ros::Publisher magnitudePub = n.advertise<lab2::magnitude>("/tri_magnitude",1000);
  
  ros::Rate loop_rate(8);
  
  pubAngleMsg.angle = 0.0;
  pubMagnitudeMsg.magnitude = 0.0;
  
  while(ros::ok()) {
    anglePub.publish(pubAngleMsg);
    magnitudePub.publish(pubMagnitudeMsg);
    
    ros::spinOnce();
    loop_rate.sleep();   
  }
  
  return 0;
}
