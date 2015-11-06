#include <ros/ros.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <ball_detector/ballLocation.h>
#include <string.h>

// Declare messages for each input
landmark_self_sim::landmarkLocation inputLandmarkMsg;
ball_detector::ballLocation inputBallMsg;

int detectBall = false;
int detectLandmark = true;

ros::Publisher ballpub;
ros::Publisher landmarkpub;

void ballCallback(const ball_detector::ballLocation::ConstPtr& msg) {
  ROS_INFO("About to try publishing %i", detectBall);
  if(detectBall > 0) {
    ROS_INFO("Publishing Message");
    ballpub.publish(*msg);
  }
}

void landmarkCallback(const landmark_self_sim::landmarkLocation::ConstPtr& msg) {
  if(detectLandmark > 0) {
    landmarkpub.publish(*msg);
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "graphics_arbitrator_node");
  ros::NodeHandle n;
  ballpub = n.advertise<ball_detector::ballLocation>("/graphics_arbitrator/ballLocation", 1000);
  landmarkpub = n.advertise<landmark_self_sim::landmarkLocation>("/graphics_arbitrator/landmarkLocation", 1000);

  ros::Subscriber ballsub = n.subscribe<ball_detector::ballLocation>("/ballLocation", 1000, ballCallback);
  ros::Subscriber landmarksub = n.subscribe<landmark_self_sim::landmarkLocation>("/landmarkLocation", 1000, landmarkCallback);

  ros::Rate loop_rate(8);

  while(ros::ok()) {
    ROS_INFO("I'm working");
    if(n.hasParam("detectBall")) 
    {            
        n.param("detectBall", detectBall);
        ROS_INFO("Set detect ball");
    }
    if(n.hasParam("detectLandmark")) n.param("detectLandmark", detectLandmark);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
