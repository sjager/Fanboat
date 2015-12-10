#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab3/fanboatControl.h>
#include <waypoint/fanboatInfo.h>
#include <math.h>

using lab3::fanboatControl;
using waypoint::fanboatInfo;

fanboatControl controlMsg;

double forwardMag = 0;

fanboatInfo infoMsg;

// fanboatInfo callback
void infoCallback(const waypoint::fanboatInfo::ConstPtr& msg) {
    infoMsg = *msg;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pursue_state_node");

    ros::NodeHandle n;

    ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/state/pursue", 1000);
    ros::Subscriber fanboatInfoSub = n.subscribe("/fanboatInfo", 1000, infoCallback);

  
    ros::Rate loop_rate(8);

	n.getParam("forwardMagnitude", forwardMag);

    while(ros::ok()) {

        /*
        //Turn to face the right way
		if((infoMsg.camAngle > 10) && (infoMsg.camCentered)) {
	        controlMsg.ignoreAngle = false;
            controlMsg.magnitude = 0;
	        controlMsg.angle = fmod((infoMsg.curAngle + infoMsg.camAngle),360);
	        controlMsg.angle = -1 * controlMsg.angle + 180;
        } else {
	        controlMsg.ignoreAngle = true;
			controlMsg.magnitude = forwardMag;
	    }
        */

        if((infoMsg.camAngle < 178) && (infoMsg.camAngle > 5)) {
            //turn right
            controlMsg.angle = infoMsg.curAngle + 120;
            controlMsg.magnitude = 0;
        } else if((infoMsg.camAngle > 182) && (infoMsg.camAngle < 355)) {
            //turn left
            controlMsg.angle = infoMsg.curAngle - 120;
            controlMsg.magnitude = 0;
        } else if(infoMsg.curCamDistance > infoMsg.tgtCamDistance) {
            controlMsg.ignoreAngle = true;
			if(infoMsg.curCamDistance > 2.0) {
				controlMsg.magnitude = forwardMag;
			} else {
				controlMsg.magnitude = forwardMag - 0.2;
			}
        } else {
			controlMsg.ignoreAngle = true;
			controlMsg.magnitude = 0;
		}


        controlPub.publish(controlMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
