#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <lab3/fanboatControl.h>

#define TURN_SPEED 15

using landmark_self_sim::landmarkLocation;
using lab3::fanboatControl;

fanboatControl pubControlMsg;
fanboat_ll::fanboatLL IMUMsg;

landmarkLocation location;
float distance;
int landmarkNumber;
double targetDistance;
bool shouldFollow = false;
float diff = 0;
float targetAngle;

bool servoMode = true;

float calculateDistance(float height) {
  float dist = 145.86*pow(height, -0.993);
  return dist;
}

void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
  IMUMsg = *msg;
  if(servoMode)
  {
    pubControlMsg.angle = IMUMsg.yaw + TURN_SPEED;
    ROS_INFO("TURN TO: %f",pubControlMsg.angle);
  }
}

void locationCallback(const landmarkLocation::ConstPtr& msg) {
  location = *msg;
  distance = calculateDistance(location.height);
    
  if(location.code == landmarkNumber) {
    ROS_INFO("\n\n-------- I FOUND IT --------\n\n");
    ROS_INFO("tgt:%i dist:%f, dist:%f",landmarkNumber, distance, diff);
  
    //fanboat should follow
    shouldFollow = true;
    servoMode = false;
    diff = targetDistance - distance;
    
    targetAngle = IMUM.yaw;
    
    pubControlMsg.angle = targetAngle;
    
    if(diff > 0) {
      pubControlMsg.magnitude =  .2;
    } else {
      pubControlMsg.magnitude = 0.0;
    }
        
  } else {
    ROS_INFO("\n\n-------- I DON'T SEE IT --------\n\n");
    //don't follow
    servoMode = true;
    shouldFollow = false;
    diff = 0;
    pubControlMsg.magnitude = 0.0;
  }
  
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "landmark_follow_node");

  ros::NodeHandle n;

  n.getParam("landmarkNumber", landmarkNumber);
  n.getParam("targetDistance", targetDistance);

  ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/fanboat_control", 1000);
  
  ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, locationCallback);
  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
  
  ros::Rate loop_rate(8);
  
  while(ros::ok()) {
    controlPub.publish(pubControlMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
