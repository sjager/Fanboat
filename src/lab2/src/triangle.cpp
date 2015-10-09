#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <math.h>

#define WAIT_TIME 5
#define FWD_MAGNITUDE .5

// Messages to be published by this node
lab2::angle pubAngleMsg;
lab2::magnitude pubMagnitudeMsg;

int main(int argc, char **argv) {

  ros::init(argc, argv, "triangle_node");
  ros::NodeHandle n;
  
  //These publishers individually publish angle and magnitude info
  ros::Publisher anglePub = n.advertise<lab2::angle>("/joy_angle", 1000);  
  ros::Publisher magnitudePub = n.advertise<lab2::magnitude>("/joy_magnitude",1000);
  
  ros::Rate loop_rate(8);
  
  pubAngleMsg.angle = 0.0;
  pubMagnitudeMsg.magnitude = 0.0;
  
  //used to toggle if the fanboat should turn or go forward
  bool turn = true;
  
  float angleVal = 0.0;
  
  while(ros::ok()) {
    if(turn) {
      //turn an additional 120 degrees.
      angleVal += 120;
      angleVal = fmod(angleVal,360.0);
      pubAngleMsg.angle = angleVal;
      pubMagnitudeMsg.magnitude = 0.0;
      ROS_INFO("turn: %f", pubAngleMsg.angle);
    } else {
      pubAngleMsg.angle = 0.0;
      pubMagnitudeMsg.magnitude = FWD_MAGNITUDE;
      ROS_INFO("forward");
    }
    anglePub.publish(pubAngleMsg);
    magnitudePub.publish(pubMagnitudeMsg);
    ros::Duration(WAIT_TIME).sleep(); //wait n seconds
    
    turn = !turn;
    ros::spinOnce();
    loop_rate.sleep();   
  }
  
  return 0;
}
