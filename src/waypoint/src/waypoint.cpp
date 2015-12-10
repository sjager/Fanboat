#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <lab3/fanboatControl.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <waypoint/fanboatInfo.h>
#include <vector>
#include <iterator>
#include <std_msgs/Int32.h>

#define SEARCH 1
#define PURSUE 2
#define AVOID  3

using lab3::fanboatControl;
using waypoint::fanboatInfo;
using std_msgs::Int32;

bool waitingForInfo = true;

std::vector<int> waypoints;
int currentWaypointIndex;

// identifies what the current state is
int currentState;

fanboatControl searchMsg;
fanboatControl pursueMsg;
fanboatControl avoidMsg;

fanboatInfo infoMsg;
Int32 tgtLandmarkPubMsg;

bool done = false;

// the message that gets selected among the three state nodes to be published
fanboatControl finalControlMsg;

void pursueCallback(const lab3::fanboatControl::ConstPtr& msg) {
    pursueMsg = *msg;
}

void avoidCallback(const lab3::fanboatControl::ConstPtr& msg) {
    avoidMsg = *msg;
}

// fanboatInfo callback
void infoCallback(const waypoint::fanboatInfo::ConstPtr& msg) {
    infoMsg = *msg;
    waitingForInfo = false;
}

void determineState() {
    // DO STATE LOGIC RIGHT HURR
    // currentState = lalala

    //If the current landmark is equal to the desired landmark, PURSUE
    if((infoMsg.curLandmark == waypoints.at(currentWaypointIndex) || infoMsg.curLandmark == -1) && infoMsg.curLandmark != 0)
    {
        currentState = PURSUE;

        //If the ir sensors see something in the way that is closer than the landmark, AVOID
        if(
         ((infoMsg.IRLDist < infoMsg.tgtIrDistance) || (infoMsg.IRRDist < infoMsg.tgtIrDistance)) &&
         ((infoMsg.IRLDist < infoMsg.curCamDistance) || (infoMsg.IRRDist < infoMsg.curCamDistance))
         )
        {
            currentState = AVOID;
        }
        //If the fanboat is finally there, it's time to find the next landmark
        else if (infoMsg.curCamDistance <= infoMsg.tgtCamDistance)
        {
            // shouldn't we be making sure that the boat is turned in the right direction?
            if(currentWaypointIndex < waypoints.size()-1)
            {
                currentWaypointIndex++;
				ROS_INFO("Index: %d", currentWaypointIndex);
			} else {
				done = true;
            }
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

    ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/fanboat_control", 1000);
    ros::Publisher tgtLandmarkPub = n.advertise<std_msgs::Int32>("/waypoint/landmark", 1000);
    
    ros::Subscriber pursueSub = n.subscribe("/state/pursue", 1000, pursueCallback);
    ros::Subscriber avoidSub  = n.subscribe("/state/avoid" , 1000, avoidCallback);

    ros::Subscriber fanboatInfoSub = n.subscribe("/fanboatInfo", 1000, infoCallback);

    n.getParam("waypoints", waypoints);
    if(waypoints.size() < 1) return -1;

    currentWaypointIndex = 0;
    
    ros::Rate loop_rate(8);

    // This code block tells the fanboat to set the target angle to its starting orientation upon startup
    /*
    while(waitingForInfo);
    ROS_INFO("reset orientation");
    
    finalControlMsg.angle = infoMsg.curAngle;
    controlPub.publish(finalControlMsg);
    */

    searchMsg.magnitude = 0;
	searchMsg.angle = infoMsg.curAngle;
    searchMsg.ignoreAngle = true;
    
    ros::Duration(2).sleep(); 

    while(ros::ok() && !done)
    {
        determineState();
        
        switch(currentState)
        {
            case SEARCH :
                finalControlMsg = searchMsg;
                ROS_INFO("SEARCH: M: %f; A: %f; I: %d", searchMsg.magnitude, searchMsg.angle, searchMsg.ignoreAngle); 
                break;
            case PURSUE :
                finalControlMsg = pursueMsg;
                ROS_INFO("PURSUE");
                break;
            case AVOID  :
                finalControlMsg = avoidMsg;
                ROS_INFO("AVOID");
                break;
            default :
                ROS_INFO("state undetermined");
        }

		ROS_INFO("Final: M: %f; A: %f; I: %d", finalControlMsg.magnitude, finalControlMsg.angle, finalControlMsg.ignoreAngle);

        ROS_INFO("Looking for: %d",waypoints.at(currentWaypointIndex));
        tgtLandmarkPubMsg.data = waypoints.at(currentWaypointIndex);
        
        controlPub.publish(finalControlMsg);
        tgtLandmarkPub.publish(tgtLandmarkPubMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }

	if(done) {
		ROS_INFO("I did it!");
	}
    
    return 0;
}
