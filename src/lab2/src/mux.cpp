#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab2/mux_control.h>

// Declare messages for each input
lab2::angle joyAngleMsg;
lab2::angle triAngleMsg;
lab2::angle reactiveAngleMsg;

lab2::magnitude joyMagnitudeMsg;
lab2::magnitude triMagnitudeMsg;
lab2::magnitude reactiveMagnitudeMsg;

//Declare final output
lab2::angle pubAngleMsg;
lab2::magnitude pubMagnitudeMsg;

// Declare mux_control input
lab2::mux_control muxControl;

void joyAngleCallback(const lab2::angle::ConstPtr& msg) {
  joyAngleMsg = *msg;
}

void joyMagnitudeCallback(const lab2::magnitude::ConstPtr& msg) {
  joyMagnitudeMsg = *msg;
}

void triAngleCallback(const lab2::angle::ConstPtr& msg) {
  triAngleMsg = *msg;
}

void triMagnitudeCallback(const lab2::magnitude::ConstPtr& msg) {
  triMagnitudeMsg = *msg;
}

void reactiveAngleCallback(const lab2::angle::ConstPtr& msg) {
  reactiveAngleMsg = *msg;
}

void reactiveMagnitudeCallback(const lab2::magnitude::ConstPtr& msg) {
  reactiveMagnitudeMsg = *msg;
}

void muxInputCallback(const lab2::mux_control::ConstPtr& msg) {
  muxControl = *msg;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "mux_node");

  ros::NodeHandle n;

  ros::Publisher anglepub = n.advertise<lab2::angle>("/target_angle", 1000);
  ros::Publisher magnitudepub = n.advertise<lab2::magnitude>("/target_magnitude", 1000);
  
  ros::Subscriber statesub = n.subscribe("/mux_control", 1000, muxInputCallback);
  
  ros::Subscriber joyanglesub = n.subscribe("/joy_angle", 1000, joyAngleCallback);
  ros::Subscriber joymagnitudesub = n.subscribe("/joy_magnitude", 1000, joyMagnitudeCallback);
  
  ros::Subscriber trianglesub = n.subscribe("/joy_angle", 1000, triAngleCallback);
  ros::Subscriber timagnitudesub = n.subscribe("/joy_magnitude", 1000, triMagnitudeCallback);
  
  ros::Rate loop_rate(8);

  pubAngleMsg.angle = 0;
  pubMagnitudeMsg.magnitude = 0;

  while(ros::ok()) {
    if(muxControl.state == 1) {
      pubAngleMsg = joyAngleMsg;
      pubMagnitudeMsg = pubMagnitudeMsg;
    } else {
      pubAngleMsg.angle = 0;
      pubMagnitudeMsg.magnitude = 0;
      joyAngleMsg.angle = 0;
      joyMagnitudeMsg.magnitude = 0;
    }
    anglepub.publish(pubAngleMsg);
    magnitudepub.publish(pubMagnitudeMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
