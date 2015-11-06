#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>

using landmark_self_sim::landmarkLocation;

landmarkLocation location;
float distance;
int landmarkNumber;
double targetDistance;
bool shouldFollow = false;
float diff = 0;



float calculateDistance(float height) {
  float dist = 145.86*pow(height, -0.993);
  return dist;
}

void locationCallback(const landmarkLocation::ConstPtr& msg) {
  location = *msg;
  distance = calculateDistance(location.height);
  
  if(location.code == landmarkNumber) {
    shouldFollow = true;
    diff = targetDistance - distance;
  } else {
    shouldFollow = false;
    diff = 0;
  }
  
  ROS_INFO("tgt:%i dist:%f, dist:%f",landmarkNumber, distance, diff);
  
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "landmark_follow_node");

  ros::NodeHandle n;

  n.getParam("landmarkNumber", landmarkNumber);
  n.getParam("targetDistance", targetDistance);

  //ros::Publisher anglepub = n.advertise<lab2::angle>("/target_angle", 1000);
  
  ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, locationCallback);
 
  ros::Rate loop_rate(8);

  
  while(ros::ok()) {
    //magnitudepub.publish(pubMagnitudeMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
