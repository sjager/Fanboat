#include <ros/ros.h>
#include <waypoint/arduinoAngle.h>
waypoint::arduinoAngle angleMessage;

int main(int argc, char **argv) {
  ros::init(argc, argv, "arduinoSweepNode");
  
  ros::NodeHandle n;
  
  
  ros::Publisher anglePub = n.advertise<waypoint::arduinoAngle>("servoAngle", 1000);
  bool increase = true;
  int iterator = 0;
  int step = 1;
  ros::Rate loop_rate(15);

  while(ros::ok()) {
    if(angleMessage.camAngle >= 360) {increase = false;}
	if(angleMessage.camAngle <= 0) {increase = true;}

	if(increase) {
      iterator = iterator + step;
      angleMessage.camAngle = iterator;
	} else {
	  iterator = iterator - step;
	  angleMessage.camAngle = iterator;
	} 
	
	anglePub.publish(angleMessage);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
