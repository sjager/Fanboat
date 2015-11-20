#include <ctime>
#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <lab3/fanboatControl.h>
#include <lab3/metrics.h>

std::clock_t timer;
double duration = 0;
double elapsed = 0;

using landmark_self_sim::landmarkLocation;
using lab3::fanboatControl;
using lab3::metrics;

ros::Publisher controlPub;

double turnSpeed;
double forwardMagnitude;

int HIGH_BOUND;
int LOW_BOUND;

fanboatControl pubControlMsg;
fanboat_ll::fanboatLL IMUMsg;
metrics distanceMsg;
landmarkLocation location;

float distance;
int landmarkNumber;
double targetDistance;
float diff = 0;
float targetAngle;

bool flipDirection = false;

// tells the fanboat to spin
bool servoMode = true;

// stops the node from sending any additional commands after the fanboat has stopped at the appropriate distance
bool done = false;

// needed to keep data in sync because the landmark data only publishes when it sees something
bool hasLandmark = false;

// keeps track of how long the fanboat has seen the target landmark.
// Used for filtering out noisy data
int consecutiveHits = 0;
int consecutiveHitsThreshold;
int hitsMax;

bool landmarkCentered = false;
int landmarkPosition = 0;

float calculateDistance(float height) {
  float dist = 145.86*pow(height, -0.993);
  return dist;
}

bool betweenBounds(landmarkLocation msg)
{

    if(msg.xtop > LOW_BOUND && msg.xtop < HIGH_BOUND)
    {
        landmarkPosition = 0;
        return true;
    }
    else if(msg.xtop >= LOW_BOUND)
    {
        landmarkPosition = -1;
        return false;
    }
    else if(msg.xtop <= HIGH_BOUND)
    {
        landmarkPosition = 1;
        return false;
    }

}

void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
  IMUMsg = *msg;
}

void locationCallback(const landmarkLocation::ConstPtr& msg) {
  location = *msg;
  hasLandmark = true;
  
  distance = calculateDistance(location.height);
  distanceMsg.header = location.header;
  distanceMsg.distance = distance;
  
  diff = distance - targetDistance;
  
  ROS_INFO("A Landmark");
  if(location.code == landmarkNumber)
  {
    flipDirection = true;
    ROS_INFO("  Correct Landmark");
    landmarkCentered = betweenBounds(location);
    if(landmarkCentered)
    {
      ROS_INFO("    Centered Landmark");
      if(diff > 0)
      {
        ROS_INFO("      Pursue Landmark: %f", diff);
        pubControlMsg.angle = IMUMsg.yaw;
        pubControlMsg.magnitude = forwardMagnitude;
        pubControlMsg.ignoreAngle = true;
        controlPub.publish(pubControlMsg);
      }
      else
      {
        ROS_INFO("      At Landmark");
        pubControlMsg.angle = IMUMsg.yaw;
        pubControlMsg.magnitude = 0;
        pubControlMsg.ignoreAngle = true;
        controlPub.publish(pubControlMsg);
      }
    }
    else
    {
      ROS_INFO("    Uncentered Landmark");
      //begin corrections
      if(landmarkPosition == 1)
      {
        ROS_INFO("      --> Landmark: %i", location.xtop);
        pubControlMsg.angle = IMUMsg.yaw - 3*turnSpeed/4;
        pubControlMsg.magnitude = 0;
        pubControlMsg.ignoreAngle = false;
        controlPub.publish(pubControlMsg);
      }
      else if(landmarkPosition == -1)
      {
        ROS_INFO("      <-- Landmark: %i", location.xtop);
        pubControlMsg.angle = IMUMsg.yaw + 3*turnSpeed/4;
        pubControlMsg.magnitude = 0;
        pubControlMsg.ignoreAngle = false;
        controlPub.publish(pubControlMsg);
      }
    }
  }
  else
  {
    ROS_INFO("  Incorrect Landmark");
    //turn half speed
    pubControlMsg.angle = IMUMsg.yaw + 3*turnSpeed/4;
    if(flipDirection)
    {
      pubControlMsg.angle = IMUMsg.yaw - 3*turnSpeed/4;
    }
    
    pubControlMsg.magnitude = 0;
    pubControlMsg.ignoreAngle = false;
    controlPub.publish(pubControlMsg);
  }
  timer = std::clock();
  elapsed = 0;
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
  
  n.getParam("HIGH_BOUND", HIGH_BOUND);
  n.getParam("LOW_BOUND", LOW_BOUND);
  
  controlPub = n.advertise<lab3::fanboatControl>("/follow/landmark", 1000);
  
  ros::Publisher metricsPub = n.advertise<lab3::metrics>("/metrics", 1000);
  
  ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, locationCallback);
  
  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
  
  ros::Rate loop_rate(4);
  
  while(ros::ok()) {

    duration = ( std::clock() - timer ) / (double) CLOCKS_PER_SEC;
    elapsed += duration;
    
    if(elapsed > 1)
    {
      pubControlMsg.angle = IMUMsg.yaw + turnSpeed;
      ROS_INFO("timer kick");
      if(flipDirection)
      {
        pubControlMsg.angle = IMUMsg.yaw - turnSpeed;
      }
      
      pubControlMsg.magnitude = 0;
      pubControlMsg.ignoreAngle = false;
      controlPub.publish(pubControlMsg);
    }    
    metricsPub.publish(distanceMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
