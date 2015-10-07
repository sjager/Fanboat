#include <ros/ros.h>
#include <lab2/angle.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab2/mux_control.h>

// Declare messages for each input
lab2::angle joyAngleMsg;
lab2::angle triAngleMsg;
lab2::angle reactiveAngleMsg;

//Declare final output
lab2::angle pubMsg;

// Declare mux_control input
lab2::mux_control muxControl;

void joyInputCallback(const lab2::angle::ConstPtr& msg) {
  joyAngleMsg = *msg;
}

void triInputCallback(const lab2::angle::ConstPtr& msg) {
  triAngleMsg = *msg;
}

void reactiveInputCallback(const lab2::angle::ConstPtr& msg) {
  reactiveAngleMsg = *msg;
}

void muxInputCallback(const lab2::mux_control::ConstPtr& msg) {
  muxControl = *msg;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "mux_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<lab2::angle>("/target_angle", 1000);
  ros::Subscriber statesub = n.subscribe("/mux_control", 1000, muxInputCallback);
  ros::Subscriber joysub = n.subscribe("/joy_angle", 1000, joyInputCallback);
  //ros::Subscriber imusub = n.subscribe("/fanboatLL", 1000, imuInputCallback);
  ros::Rate loop_rate(8);

  pubMsg.angle = 0;
  
  while(ros::ok()) {
    if(muxControl.state == 1) {
      pubMsg = joyAngleMsg;
    } else {
      pubMsg.angle = 0;
      joyAngleMsg.angle = 0;
    }
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
