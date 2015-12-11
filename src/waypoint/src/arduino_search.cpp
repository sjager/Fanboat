//Import needed messages and stuff
#include <ros/ros.h>
#include <waypoint/arduinoAngle.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <waypoint/fanboatInfo.h>
waypoint::arduinoAngle angleMessage;

using landmark_self_sim::landmarkLocation;

//Set up some global variables
landmarkLocation location;

bool increase = true;
double iterator = 0;
double step = 2.0;
int targetCode = 68;

//Set up some things that should be constants
int HIGH_BOUND = 400;
int LOW_BOUND = 240;
int BIG_STEP = 1;
double SMALL_STEP = 0.5;


//Function 
void locationCallback(const landmarkLocation::ConstPtr& msg) {
  location = *msg;
  //ROS_INFO("Found something");
  if(location.code == -1 || location.code == targetCode) {
    step = SMALL_STEP;
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

void codeCallback(const waypoint::fanboatInfo::ConstPtr& msg) {
  if(targetCode != msg->tgtLandmark) {
	targetCode = msg->tgtLandmark;
	step = BIG_STEP;
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "arduino_search_node");
  
  ros::NodeHandle n;
  
  ros::Publisher anglePub = n.advertise<waypoint::arduinoAngle>("servoAngle", 10);
  ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 10, locationCallback);
  ros::Subscriber fanboatInfo = n.subscribe("/fanboatInfo", 10, codeCallback);
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
