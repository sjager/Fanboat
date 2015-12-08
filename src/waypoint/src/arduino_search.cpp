#include <ros/ros.h>
#include <waypoint/arduinoAngle.h>
#include <landmark_self_sim/landmarkLocation.h>
waypoint::arduinoAngle angleMessage;

using landmark_self_sim::landmarkLocation;

landmarkLocation location;

bool increase = true;
double iterator = 0;
double step = 2.0;
int targetCode = 68;
int HIGH_BOUND = 370;
int LOW_BOUND = 270;

void locationCallback(const landmarkLocation::ConstPtr& msg) {
  location = *msg;
  //ROS_INFO("Found something");
  if(location.code == -1 || location.code == targetCode) {
    step = 1;
    if(location.code == targetCode) {
	  //ROS_INFO("Found it");
	  if(location.xtop > HIGH_BOUND) {
		  ROS_INFO("Not far enough, keep going");
		  increase = true;
	  } else if(location.xtop < LOW_BOUND) {
		  ROS_INFO("Too far, turning back");
		  increase = false;
 	  } else {
		  ROS_INFO("Locked on");
		  step = 0;
   	  }
    }
  }
} 

int main(int argc, char **argv) {
  ros::init(argc, argv, "arduinoSweepNode");
  
  ros::NodeHandle n;
  
  ros::Publisher anglePub = n.advertise<waypoint::arduinoAngle>("servoAngle", 1000);
  ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, locationCallback);
  ros::Rate loop_rate(10);
  while(ros::ok()) {
    if(angleMessage.camAngle >= 360) {increase = false;}
	if(angleMessage.camAngle <= 0) {increase = true;}

	if(increase) {
      iterator = iterator + step;
      angleMessage.camAngle = int(iterator);
	} else {
	  iterator = iterator - step;
	  angleMessage.camAngle = int(iterator);
	} 

	if(step == 0) {
	  angleMessage.centered = true;
	} else {
	  angleMessage.centered = false;
	}
	
	anglePub.publish(angleMessage);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
