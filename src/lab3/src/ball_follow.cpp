#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab3/fanboatControl.h>
#include <ball_detector/ballLocation.h>

//#define HIGH_BOUND   260 //right
//#define LOW_BOUND   -270 //left

using ball_detector::ballLocation;
using lab3::fanboatControl;

int HIGH_BOUND;
int LOW_BOUND;

double turnSpeed;
double forwardMagnitude;
double speedFraction;

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
bool ballCentered = false;

// stops the node from sending any additional commands after the fanboat has stopped at the appropriate distance
bool done = false;

int ballDirection = 0;

// keeps track of how long the fanboat has seen the target landmark.
// Used for filtering out noisy data
int consecutiveHits = 0;
int consecutiveHitsThreshold;
int hitsMax;

int radiusThreshold = 100;

float calculateBallDistance(float radius)
{
	float dist = 0.01*4887.4*pow(radius, -1.071);
	return dist;
}

bool ballAtCenter(float x)
{
  //ROS_INFO("X: %f", x);

	if(x < HIGH_BOUND && x > LOW_BOUND)
	{
	    ballDirection = 0;
    	return true;
	}
	else if(x >= HIGH_BOUND)
	{
	    ballDirection = 1;
    	return false;
	}
	else if(x <= LOW_BOUND)
	{
	    ballDirection = -1;
	    return false;
	}
}

void ballCallback(const ball_detector::ballLocation::ConstPtr& msg) {
  
  bLocation = *msg;
  
  if(bLocation.radius > radiusThreshold) {
    seesBall = true;
    bDistance = calculateBallDistance(bLocation.radius);
    diff = bDistance - targetDistance;
    ballCentered = ballAtCenter(bLocation.x);
  } else {
    seesBall = false;
  }
  
}

void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
  IMUMsg = *msg;
  //ROS_INFO("Current Angle: %f", IMUMsg.yaw);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "ball_follow_node");

  ros::NodeHandle n;

  n.getParam("speedFraction", speedFraction);
  n.getParam("targetDistance", targetDistance);
  
  n.getParam("turnSpeed", turnSpeed);
  n.getParam("forwardMagnitude", forwardMagnitude);
  
  n.getParam("consecutiveHitsThreshold", consecutiveHitsThreshold);
  n.getParam("hitsMax", hitsMax);
  
  n.getParam("radiusThreshold", radiusThreshold);
  
  n.getParam("HIGH_BOUND", HIGH_BOUND);
  n.getParam("LOW_BOUND", LOW_BOUND);
  
  ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/follow/ball", 1000);
  
  ros::Subscriber ballSub = n.subscribe("ballLocation", 1000, ballCallback);
  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
  
  ros::Rate loop_rate(10);
  
  while(ros::ok()) {
    //if the ball is visible
    
    if(seesBall && !done)
    {
      ROS_INFO("X: %f",bLocation.x);
      ROS_INFO("Centered: %i", ballCentered);    
      //ROS_INFO("I see a ball");
      //if the ball isn't centered
      if(!ballCentered)
      {
        //ROS_INFO("The ball ain't centered");
        //rotate
        if(ballDirection == -1)
        {
          // ball on left
          pubControlMsg.ignoreAngle = false;
          pubControlMsg.angle = IMUMsg.yaw - turnSpeed * speedFraction;
          pubControlMsg.magnitude = 0;  
          ROS_INFO("<--");
        }
        else if (ballDirection == 1)
        {
          // ball on right
          pubControlMsg.ignoreAngle = false;
          pubControlMsg.angle = IMUMsg.yaw + turnSpeed * speedFraction;
          pubControlMsg.magnitude = 0;
          ROS_INFO("-->");
        }
        
      }
      else //ball is centered
      {
        consecutiveHits++;
        
        ROS_INFO("Go forth!");
        //go forward
        pubControlMsg.angle = IMUMsg.yaw;
        pubControlMsg.magnitude = forwardMagnitude;
        pubControlMsg.ignoreAngle = true;
      }
    }
    else if(!seesBall && !done && (consecutiveHits <= 15)) //was consecutiveHitsThreshold
    {
      //ROS_INFO("I don't see a ball :(");
      pubControlMsg.magnitude = 0;
      pubControlMsg.ignoreAngle = false;
      pubControlMsg.angle = turnSpeed + IMUMsg.yaw;
    }
    else if(!seesBall && !done && (consecutiveHits > 15)) //was consecutiveHitsThreshold
    {
      //the ball is probably in the catcher and not visible to the camera
        pubControlMsg.magnitude = forwardMagnitude;
        pubControlMsg.ignoreAngle = true;
        pubControlMsg.angle = IMUMsg.yaw;
    }
    
    //ROS_INFO("msg angle: %f", pubControlMsg.angle);
    
    controlPub.publish(pubControlMsg);
    seesBall = false;
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
