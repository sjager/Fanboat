#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <lab3/fanboatControl.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <waypoint/fanboatInfo.h>

using std_msgs::float32;
using std_msgs::int32;

float tgtAngle;         // fanboatcontrol
float tgtMagnitude;     // fanboatcontrol
int tgtLandmark;        // waypoint (?)
float tgtCamDistance;   // launch
float tgtIrDistance;    // launch

float curAngle;         // IMU
float curMagnitude;     // IMU
bool ignoreAngle;       // fanboatcontrol

int curLandmark;        // landmark
float curCamDistance;   // landmark
float curIrDistance;    // IR

float motorR;           // motors
float motorL;           // motors

float IRL;              // IR
float IRR;              // IR

float camAngle;         // camServo

/*
void searchCallback(const lab3::fanboatControl::ConstPtr& msg) {
    searchMsg = *msg;
}
*/

int main(int argc, char **argv) {

    ros::init(argc, argv, "fanboat_info_node");

    ros::NodeHandle n;

    ros::Publisher infoPub = n.advertise<lab3::fanboatInfo>("/fanboatInfo", 1000);
    
    ros::Subscriber controlSub = n.subscribe("/waypoint/control", 1000, controlCallback);
    
    ros::Subscriber tgtLandmarkSub = n.subscribe("/waypoint/landmark", 1000, controlCallback);
    
    ros::Subscriber IMUSub = n.subscribe("/fanboatLL", 1000, controlCallback);
    
    ros::Subscriber landmarkSub = n.subscribe("/waypoint/control", 1000, controlCallback);

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
