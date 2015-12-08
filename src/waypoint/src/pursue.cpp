#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <lab3/fanboatControl.h>
#include <waypoint/fanboatInfo.h>

using lab3::fanboatControl;
using waypoint::fanboatInfo;

fanboatControl controlMsg;

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

    while(ros::ok()) {

        if(infoMsg.curCamDistance>infoMsg.tgtCamDistance)
        {
            //drive forward; increase magnitude
            controlMsg.magnitude = infoMsg.tgtMagnitude;
        }

        //Should this node even worry about angles?
        if(infoMsg.tgtAngle!=infoMsg.curAngle)
        {
            controlMsg.angle = infoMsg.tgtAngle;
            controlMsg.ignoreAngle = false;
        }
        else
        {
            controlMsg.ignoreAngle = true;
        }

        controlPub.publish(controlMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
