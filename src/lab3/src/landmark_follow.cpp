#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <lab3/fanboatControl.h>

//#define TURN_SPEED 60
//#define FORWARD_MAGNITUDE .9

using landmark_self_sim::landmarkLocation;
using lab3::fanboatControl;

double turnSpeed;
double forwardMagnitude;

fanboatControl pubControlMsg;
fanboat_ll::fanboatLL IMUMsg;

landmarkLocation location;
float distance;
int landmarkNumber;
double targetDistance;
bool shouldFollow = false;
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

float calculateDistance(float height) {
  float dist = 145.86*pow(height, -0.993);
  return dist;
}

void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
  IMUMsg = *msg;
  if(servoMode && !done) {
    pubControlMsg.angle = IMUMsg.yaw + turnSpeed;
    ROS_INFO("TURN TO: %f",pubControlMsg.angle);
  } else {
    ROS_INFO("\n\n-------- I FOUND IT --------");
  }
}

void locationCallback(const landmarkLocation::ConstPtr& msg) {
  location = *msg;
  distance = calculateDistance(location.height);
    
  if(location.code == landmarkNumber) {
    ROS_INFO("\n\n-------- I FOUND IT --------");
    ROS_INFO("tgt:%i dist:%f, diff:%f",landmarkNumber, distance, diff);
  
    consecutiveHits++;
  
    //fanboat should follow
    shouldFollow = true;
    servoMode = false;
    diff = distance - targetDistance;
    
    targetAngle = IMUMsg.yaw;
    
    pubControlMsg.angle = targetAngle;
    
    if((diff > 0) && (!done)) {
      pubControlMsg.magnitude =  forwardMagnitude;
    } else {
      pubControlMsg.magnitude = 0.0;
      done = true;
      ROS_INFO("\n\n-------- DONE --------");
    }
        
  } else {
  
    ROS_INFO("\n\n-------- I DON'T SEE IT --------\n\n");
    
    consecutiveHits--;
    
    // often the camera fails to recognize the image as the correct landmark,
    // causing the fanboat to stutter. We can use a consecutiveHits counter to
    // attempt to smooth out the stuttering. The logic is that if the robot has frequently
    // recognized the landmark, then it probably is actually facing that landmark.
    if(consecutiveHits < consecutiveHitsThreshold)
    {
      //don't follow
      servoMode = true;
      shouldFollow = false;
      diff = 0;
      pubControlMsg.magnitude = 0.0;
    }
    
  }
  
  if(consecutiveHits > hitsMax)
  {
    consecutiveHits = hitsMax;
  }
  else if (consecutiveHits < 0)
  {
    consectuiveHits = 0;
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
