#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab3/fanboatControl.h>
#include <ball_detector/ballLocation.h>

#define HIGH_BOUND   360
#define LOW_BOUND  280

using ball_detector::ballLocation;
using lab3::fanboatControl;

double turnSpeed;
double forwardMagnitude;

fanboatControl pubControlMsg;
fanboat_ll::fanboatLL IMUMsg;

ballLocation bLocation;

float bDistance;
double targetDistance;
float diff = 0;
float targetAngle;

// tells the fanboat to spin
bool servoMode = true;

// stops the node from sending any additional commands after the fanboat has stopped at the appropriate distance
bool done = false;

// keeps track of how long the fanboat has seen the target landmark.
// Used for filtering out noisy data
int consecutiveHits = 0;
int consecutiveHitsThreshold;
int hitsMax;

bool betweenBounds(landmarkLocation msg)
{
    if(msg.xtop > 280 && msg.xtop < 360)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float calculateBallDistance(float radius)
{
	float dist = 0.01*4887.4*pow(radius, -1.071)
	return dist;
}

bool ballAtCenter(float x)
{
	if(x>HIGH_BOUND && x<LOW_BOUND)
	{
    	return true;
	}
	else
	{
    	return false;
	}
}

void ballCallback(const ball_detector::ballLocation::ConstPtr& msg) {
  bLocation = *msg;
  bDistance = calculateBallDistance(bLocation.radius);  
}

void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
  IMUMsg = *msg;
  if(servoMode) {  
    pubControlMsg.angle = IMUMsg.yaw + turnSpeed;
    //ROS_INFO("TURN TO: %f",pubControlMsg.angle);
    
  } else {
    pubControlMsg.angle = IMUMsg.yaw;
    //ROS_INFO("\n\n-------- DON'T SPIN --------");
  }
}

void locationCallback(const landmarkLocation::ConstPtr& msg) {
  hasLandmark = true;
  location = *msg;
  distance = calculateDistance(location.height);
  pubControlMsg.ignoreAngle = false;
  
  if(consecutiveHits > hitsMax)
  {
    consecutiveHits = hitsMax;
  }
  else if (consecutiveHits < 0)
  {
    consecutiveHits = 0;
  }
  
  if(location.code == landmarkNumber) {
    //ROS_INFO("\n\n-------- I FOUND IT --------");
    //ROS_INFO("tgt:%i dist:%f, diff:%f",landmarkNumber, distance, diff);
  
    consecutiveHits++;
  
    //fanboat should follow
    servoMode = false;
    diff = distance - targetDistance;
    
    //stop rotating
    pubControlMsg.angle = IMUMsg.yaw;
    
    if((diff > 0)) {

      pubControlMsg.magnitude =  forwardMagnitude;
      pubControlMsg.ignoreAngle = true;
      done = false;
      ROS_INFO("\n----Keep Going!----\n");
    } else {
      pubControlMsg.magnitude = 0.0;
      //done = true;
      //ROS_INFO("\n\n-------- DONE --------");
    }
        
  } else {
    //it doesn't see the correct landmark
  
    ROS_INFO("\n\n-------- I DON'T SEE IT --------\n\n");
    
    consecutiveHits--;
    
    // often the camera fails to recognize the image as the correct landmark,
    // causing the fanboat to stutter. We can use a consecutiveHits counter to
    // attempt to smooth out the stuttering. The logic is that if the robot has frequently
    // recognized the landmark, then it probably is actually facing that landmark.
    
    if(consecutiveHits >= consecutiveHitsThreshold)
    {
      ROS_INFO("ignore noise\n");
    }
    if(consecutiveHits < consecutiveHitsThreshold)
    {
      //don't follow
      servoMode = true;
      diff = 0;
      pubControlMsg.magnitude = 0.12;
    }
    
  }
  
  ROS_INFO("counter: %i", consecutiveHits);
  hasLandmark = false;
}

void printMode()
{
  if(servoMode)
  {
    ROS_INFO("servoing");
  }
  else
  {
    ROS_INFO("following");
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "landmark_follow_node");

  ros::NodeHandle n;

  n.getParam("landmarkNumber", landmarkNumber);
  n.getParam("targetDistance", targetDistance);
  
  n.getParam("turnSpeed", turnSpeed);
  n.getParam("forwardMagnitude", forwardMagnitude);
  
  n.getParam("consecutiveHitsThreshold", consecutiveHitsThreshold);
  n.getParam("hitsMax", hitsMax);
  
  n.setParam("detectLandmark", 1);
 
  ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/fanboat_control", 1000);
  
  ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, locationCallback);
  ros::Subscriber ballSub = n.subscribe("ballLocation", 1000, ballCallback);
  
  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
  
  
  ros::Rate loop_rate(4);
  
  while(ros::ok()) {
    
    if(!hasLandmark)
    {
      consecutiveHits = 0;
      servoMode = true;
    }
  
    controlPub.publish(pubControlMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
