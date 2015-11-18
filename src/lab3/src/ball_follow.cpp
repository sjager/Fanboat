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

float calculateBallDistance(float radius)
{
	float dist = 0.01*4887.4*pow(radius, -1.071);
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
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "ball_follow_node");

  ros::NodeHandle n;

  n.getParam("targetDistance", targetDistance);
  
  n.getParam("turnSpeed", turnSpeed);
  n.getParam("forwardMagnitude", forwardMagnitude);
  
  n.getParam("consecutiveHitsThreshold", consecutiveHitsThreshold);
  n.getParam("hitsMax", hitsMax);
  
  ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/follow/ball", 1000);
  
  ros::Subscriber ballSub = n.subscribe("ballLocation", 1000, ballCallback);
  
  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
  
  ros::Rate loop_rate(4);
  
  while(ros::ok()) {
    pubControlMsg.ignoreAngle = false;
    
    //if the ball is visible
    if(seesBall)
    {
      ROS_INFO("I see a ball\n");
      //if the ball isn't centered
      if(!ballCentered)
      {
        ROS_INFO("The ball ain't centered\n");
        //rotate
        pubControlMsg.angle = turnSpeed + IMUMsg.yaw;
      }
      else //ball is centered
      {
        if(!done)
        {
          ROS_INFO("Go forth!\n");
          //go forward
          pubControlMsg.magnitude = forwardMagnitude;
          pubControlMsg.ignoreAngle = true;
        }
      }
    }
    else
    {
    ROS_INFO("I don't see a ball :(\n");
    pubControlMsg.angle = turnSpeed + IMUMsg.yaw;
    }  
    
    controlPub.publish(pubControlMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
