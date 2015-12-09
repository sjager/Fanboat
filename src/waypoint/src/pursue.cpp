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

        //Turn to face the right way
		if((infoMsg.camAngle > 10) && (infoMsg.camCentered)) {
	        controlMsg.angle = fmod((infoMsg.curAngle + infoMsg.camAngle),360);
	        controlMsg.ignoreAngle = false;
	    } else if (infoMsg.camCentered) {
	        controlMsg.ignoreAngle = true;
			controlMsg.magnitude = forwardMag;
	    }

        controlPub.publish(controlMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
