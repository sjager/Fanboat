#include <ros/ros.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <ball_detector/ballLocation.h>
#include <string.h>

// Declare messages for each input
landmark_self_sim::landmarkLocation inputLandmarkMsg;
ball_detector::ballLocation inputBallMsg;

ros::NodeHandle n;
ros::Publisher ballpub;
ros::Publisher landmarkpub;

void ballCallback(const ball_detector::ballLocation::ConstPtr& msg) {
  bool detectBall = false;
  if(n.hasParam("detectBall") && n.param("detectBall", detectBall)) {
    ballpub.publish(*msg);
  }
}

void landmarkCallback(const landmark_self_sim::landmarkLocation::ConstPtr& msg) {
  bool detectLandmark = false;
  if(n.hasParam("detectLandmark") && n.param("detectLandmark", detectLandmark)) {
    landmarkpub.publish(*msg);
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "graphics_arbitrator_node");

  ballpub = n.advertise<ball_detector::ballLocation>("/graphics_arbitrator/", 1000);
  landmarkpub = n.advertise<landmark_self_sim::landmarkLocation>("/graphics_arbitrator/landmarkLocation", 1000);

  ros::Subscriber ballsub = n.subscribe<ball_detector::ballLocation>("/ballLocation", 1000, ballCallback);
  ros::Subscriber landmarksub = n.subscribe<landmark_self_sim::landmarkLocation>("/landmarkLocation", 1000, landmarkCallback);

  ros::Rate loop_rate(8);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
