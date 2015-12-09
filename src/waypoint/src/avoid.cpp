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

int turnBy = 15;

// fanboatInfo callback
void infoCallback(const waypoint::fanboatInfo::ConstPtr& msg) {
    infoMsg = *msg;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "avoid_state_node");

    ros::NodeHandle n;

    ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/state/avoid", 1000);

    ros::Subscriber fanboatInfoSub = n.subscribe("/fanboatInfo", 1000, infoCallback);
  
    ros::Rate loop_rate(8);

    while(ros::ok()) {

        // Check that the obstacle is in front of us. 
        // If it is, turn more.
        if((infoMsg.IRRDist < infoMsg.tgtIrDistance) || (infoMsg.IRRDist < infoMsg.tgtIrDistance))
        {
            controlMsg.angle = infoMsg.curAngle + turnBy;
        }
        else //Otherwise, turn back to the angle we were moving towards the landmark at.
        {
            //NOTE: This assumes that the target angle is the angle we need to move at the landmark at.
            controlMsg.angle = infoMsg.tgtAngle;
        }

        //Set the boat to half speed
        controlMsg.magnitude = infoMsg.tgtMagnitude / 2;

        controlPub.publish(controlMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
