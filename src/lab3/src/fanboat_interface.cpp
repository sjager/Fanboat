#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab3/fanboatControl.h>

using lab3::fanboatControl;

fanboatControl controlMsg;

// Declare messages for each input
lab2::magnitude pubMagnitudeMsg;
lab2::angle pubAngleMsg;

void controlInputCallback(const lab3::fanboatControl::ConstPtr& msg) {
  controlMsg = *msg;
  pubMagnitudeMsg.magnitude = controlMsg.magnitude;
  pubAngleMsg.angle = controlMsg.angle;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "fanboat_interface_node");

  ros::NodeHandle n;

  ros::Publisher anglepub = n.advertise<lab2::angle>("/target_angle", 1000);
  ros::Publisher magnitudepub = n.advertise<lab2::magnitude>("/target_magnitude", 1000);
  
  ros::Subscriber controlSub = n.subscribe("/fanboat_control", 1000, controlInputCallback);
  
  ros::Rate loop_rate(8);

  while(ros::ok()) {

    anglepub.publish(pubAngleMsg);
    magnitudepub.publish(pubMagnitudeMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
