#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <string.h>
int detectBall = false;
int detectLandmark = false;

ros::Publisher ballpub;
ros::Publisher landmarkpub;

void imageCallback(const sensor_msgs::Image::ConstPtr& msg) {
  //ROS_INFO("About to try publishing %i", detectBall);
  if(detectBall > 0) {
    ballpub.publish(*msg);
  }

  if(detectLandmark > 0) {
    landmarkpub.publish(*msg);
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "graphics_arbitrator_node");
  ros::NodeHandle n;
  ballpub = n.advertise<sensor_msgs::Image>("/graphics_arbitrator/ballImage", 1000);
  landmarkpub = n.advertise<sensor_msgs::Image>("/graphics_arbitrator/landmarkImage", 1000);

  ros::Subscriber ballsub = n.subscribe<sensor_msgs::Image>("/usb_cam/image_raw", 1000, imageCallback);
  //ros::Subscriber landmarksub = n.subscribe<landmark_self_sim::landmarkLocation>("/landmarkLocation", 1000, landmarkCallback);

  ros::Rate loop_rate(8);

  while(ros::ok()) {
    ROS_INFO("I'm working");
    if(n.hasParam("detectBall")) 
    {            
        n.getParam("detectBall", detectBall);
    }
    if(n.hasParam("detectLandmark"))
    { 
        n.getParam("detectLandmark", detectLandmark);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
