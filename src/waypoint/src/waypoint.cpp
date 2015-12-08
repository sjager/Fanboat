#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <lab3/fanboatControl.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <waypoint/fanboatInfo.h>

#define SEARCH 1
#define PURSUE 2
#define AVOID  3

using lab3::fanboatControl;
using waypoint::fanboatInfo;

// identifies what the current state is
int currentState;

fanboatControl searchMsg;
fanboatControl pursueMsg;
fanboatControl avoidMsg;

fanboatInfo infoMsg;

// the message that gets selected among the three state nodes to be published
fanboatControl finalControlMsg;

// state node callbacks
void searchCallback(const lab3::fanboatControl::ConstPtr& msg) {
    searchMsg = *msg;
}

void pursueCallback(const lab3::fanboatControl::ConstPtr& msg) {
    pursueMsg = *msg;
}

void avoidCallback(const lab3::fanboatControl::ConstPtr& msg) {
    avoidMsg = *msg;
}


// fanboatInfo callback
void infoCallback(const waypoint::fanboatInfo::ConstPtr& msg) {
    infoMsg = *msg;
}


void determineState() {
    // DO STATE LOGIC RIGHT HURR
    // currentState = lalala

    //If the current landmark is equal to the desired landmark, PURSUE
    if(infoMsg.curLandmark == infoMsg.tgtLandmark)
    {
        currentState = PURSUE;

        //If the ir sensors see something in the way that is closer than the landmark, AVOID
        if(infoMsg.curIrDistance < infoMsg.tgtIrDistance 
            && infoMsg.curIrDistance < infoMsg.curCamDistance)
        {
            currentState = AVOID;
        }
    }
    else //Otherwise, SEARCH
    {
        currentState = SEARCH;
    }

}

int main(int argc, char **argv) {

    ros::init(argc, argv, "waypoint_node");

    ros::NodeHandle n;

    ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/waypoint/control", 1000);
    
    ros::Subscriber searchSub = n.subscribe("/state/search", 1000, searchCallback);
    ros::Subscriber pursueSub = n.subscribe("/state/pursue", 1000, pursueCallback);
    ros::Subscriber avoidSub  = n.subscribe("/state/avoid" , 1000, avoidCallback);

    ros::Subscriber fanboatInfoSub = n.subscribe("/fanboatInfo", 1000, infoCallback);

    ros::Rate loop_rate(8);

    while(ros::ok())
    {
        determineState();
        
        switch(currentState)
        {
            case SEARCH :
                finalControlMsg = searchMsg; 
                break;
            case PURSUE :
                finalControlMsg = pursueMsg;
                break;
            case AVOID  :
                finalControlMsg = avoidMsg;
                break;
            default :
                ROS_INFO("state undetermined");
        }
        
        controlPub.publish(finalControlMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
