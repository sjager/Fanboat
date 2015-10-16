#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab2/mux_control.h>
#include <lab2/toggle.h>

// Declare messages for each input
lab2::toggle mapSwitchMsg;
lab2::toggle reactSwitchMsg; 

// Declare mux_control input
lab2::mux_control muxControl;

// Declare output
lab2::toggle pubMsg;

void mapSwitchCallback(const lab2::toggle::ConstPtr& msg) {
  mapSwitchMsg = *msg;
}

void reactSwitchCallback(const lab2::toggle::ConstPtr& msg) {
  reactSwitchMsg = *msg;
}

void muxInputCallback(const lab2::mux_control::ConstPtr& msg) {
  muxControl = *msg;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "mapping_mux_node");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<lab2::toggle>("/mapping_toggle_control", 1000);
  
  ros::Subscriber statesub = n.subscribe("/mux_control", 1000, muxInputCallback);
  
  ros::Subscriber mapSwitchSub = n.subscribe("/map_switch", 1000, mapSwitchCallback);
  ros::Subscriber reactSwtichSub = n.subscribe("/react_switch", 1000, reactSwitchCallback);
  
  ros::Rate loop_rate(8);


  while(ros::ok()) {
    ROS_INFO("Map switch toggle: %d, React switch toggle: %d, Mux Control: %d", mapSwitchMsg.toggle, reactSwitchMsg.toggle, muxControl.state);
    if(muxControl.state == 4) {
      if(!pubMsg.toggle && mapSwitchMsg.toggle) pubMsg.toggle = true;
      else if(pubMsg.toggle && reactSwitchMsg.toggle) pubMsg.toggle = false;
    } else {
      pubMsg.toggle = false;
    }

    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
