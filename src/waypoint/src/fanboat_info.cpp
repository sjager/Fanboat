#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <lab3/fanboatControl.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <waypoint/fanboatInfo.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>

using std_msgs::Float32;
using std_msgs::Int32;
using waypoint::fanboatInfo;

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

fanboatInfo fbInfo;

void controlCallback(const lab3::fanboatControl::ConstPtr& msg) {
}

void tgtLandmarkCallback(const std_msgs::Int32::ConstPtr& msg) {
}

void IMUCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
}

void landmarkCallback(const landmark_self_sim::landmarkLocation::ConstPtr& msg) {
}

/*
void IMUCallback(const waypoint::camera?::ConstPtr& msg) {
}
*/

int main(int argc, char **argv) {

    ros::init(argc, argv, "fanboat_info_node");

    ros::NodeHandle n;

    ros::Publisher infoPub = n.advertise<waypoint::fanboatInfo>("/fanboatInfo", 1000);
    
    ros::Subscriber controlSub = n.subscribe("/waypoint/control", 1000, controlCallback);
    
    ros::Subscriber tgtLandmarkSub = n.subscribe("/waypoint/landmark", 1000, tgtLandmarkCallback);
    
    ros::Subscriber IMUSub = n.subscribe("/fanboatLL", 1000, IMUCallback);
    
    ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, landmarkCallback);
    
    //ros::Subscriber cameraSub = n.subscribe("/waypoint/control", 1000, cameraCallback);

    ros::Rate loop_rate(8);

    while(ros::ok())
    {        
        infoPub.publish(fbInfo);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
