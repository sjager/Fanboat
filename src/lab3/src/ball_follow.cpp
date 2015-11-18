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

// true if the camera sees a ball
bool seesBall = false;

// true if ball is centered in the cam's view
bool ballCentered = true;

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
  seesBall = true;
  bLocation = *msg;
  bDistance = calculateBallDistance(bLocation.radius);
  diff = bDistance - targetDistance;
  ballCentered = ballAtCenter(bLocation.x);
  seesBall = false;
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
 
  ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/follow/ball", 1000);
  
  ros::Subscriber ballSub = n.subscribe("ballLocation", 1000, ballCallback);
  
  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
  
  ros::Rate loop_rate(4);
  
  while(ros::ok()) {
    pubControlMsg.ignoreAngle = false;
    
    //if the ball is visible
    if(seesBall)
    {
      //if the ball isn't centered
      if(ballCentered)
      {
        //rotate
        pubControlMsg.angle = turnSpeed + IMUMsg.yaw;
      }
      else //ball is centered
      {
        if(!done)
        {
          //go forward
          pubControlMsg.magnitude = forwardMagnitude;
          pubControlMsg.ignoreAngle = true;
        }
      }
    }
    else
    {
      pubControlMsg.angle = turnSpeed + IMUMsg.yaw;
    }  
    
    controlPub.publish(pubControlMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
