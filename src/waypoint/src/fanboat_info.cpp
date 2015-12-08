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

// HNNNGG SO MANY USINGS
using std_msgs::Float32;
using std_msgs::Int32;
using waypoint::fanboatInfo;
using lab3::fanboatControl;
using fanboat_ll:fanboatLL;
using fanboat_ll:fanboatMotors;
using landmark_self_sim::landmarkLocation;

/*
float tgtAngle;         // fanboatcontrol
float tgtMagnitude;     // fanboatcontrol
int tgtLandmark;        // waypoint (?)
float tgtCamDistance;   // launch
float tgtIrDistance;    // launch

float curAngle;         // IMU
bool ignoreAngle;       // fanboatcontrol

int curLandmark;        // landmark
float curCamDistance;   // landmark
float curIrDistance;    // IR

float motorR;           // motors
float motorL;           // motors

float IRL;              // IR
float IRR;              // IR

float camAngle;         // camServo
*/

// message to be published
fanboatInfo fbInfo;

// subscribed messages
fanboatControl controlMsg;
Int32 tgtLandmarkMsg;
fanboatLL IMUMsg;
fanboatMotors motorMsg;
landmarkLocation landmarkLocationMsg;

void controlCallback(const lab3::fanboatControl::ConstPtr& msg) {
    controlMsg = *msg; 
}

void tgtLandmarkCallback(const std_msgs::Int32::ConstPtr& msg) {
    tgtLandmarkMsg = *msg;
}

void IMUCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
    IMUMsg = *msg;
}

void motorCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
    motorMsg = *msg;
}

// so this might actually not be the right place to get this info. 
// maybe from a different post-landmark processing node? 
/*
void landmarkCallback(const landmark_self_sim::landmarkLocation::ConstPtr& msg) {
    landmarkLocationMsg = *msg;
    
    curLandmark = landmarkLocationMsg.code;
}
*/

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
    
    ros::Subscriber motorSub = n.subscribe("/motors", 1000, motorCallback);
    
    /*
    ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, landmarkCallback);
    
    ros::Subscriber IRSub = n.subscribe("/landmarkLocation", 1000, landmarkCallback);
    */
    
    //ros::Subscriber cameraSub = n.subscribe("/waypoint/control", 1000, cameraCallback);

    ros::Rate loop_rate(8);

    while(ros::ok())
    {        
    
        fbInfo.tgtAngle = controlMsg.angle;
        fbInfo.tgtMagnitude = controlMsg.magnitude;
        fbInfo.ignoreAngle = controlMsg.ignoreAngle;
    
        fbInfo.tgtLandmark = tgtLandmarkMsg.data; // .data, right?
        
        fbInfo.curAngle = IMUMsg.yaw;       
 
        fbInfo.motorR = motorMsg.right;
        fbInfo.motorL = motorMsg.left;
           
        //fbInfo.tgtCamDistance
        //fbInfo.tgtIrDistance
        
        //fbInfo.curLandmark
        //fbInfo.curCamDistance
        //fbInfo.curIrDistance
        //fbInfo.IRL
        //fbInfo.IRR
        
        //fbInfo.camAngle
        
        infoPub.publish(fbInfo);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
