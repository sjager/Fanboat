#include <ros/ros.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <lab2/range.h>
#include <lab3/fanboatControl.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <waypoint/fanboatInfo.h>
#include <waypoint/landmarkInfo.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <waypoint/arduinoAngle.h>

// HNNNGG SO MANY USINGSSS
using std_msgs::Float32;
using std_msgs::Int32;
using waypoint::fanboatInfo;
using waypoint::landmarkInfo;
using lab3::fanboatControl;
using fanboat_ll::fanboatLL;
using fanboat_ll::fanboatMotors;
using landmark_self_sim::landmarkLocation;
using lab2::range;
using waypoint::arduinoAngle;

// message to be published
fanboatInfo fbInfo;

// subscribed messages
fanboatControl controlMsg;
Int32 tgtLandmarkMsg;
fanboatLL IMUMsg;
fanboatMotors motorMsg;
range IRMsg;
landmarkInfo landmarkInfoMsg;
arduinoAngle arduinoInfoMsg;

void controlCallback(const lab3::fanboatControl::ConstPtr& msg) {
    controlMsg = *msg; 
}

void tgtLandmarkCallback(const std_msgs::Int32::ConstPtr& msg) {
    tgtLandmarkMsg = *msg;
}

void IMUCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
    IMUMsg = *msg;
}

void motorCallback(const fanboat_ll::fanboatMotors::ConstPtr& msg) {
    motorMsg = *msg;
}

void landmarkCallback(const waypoint::landmarkInfo::ConstPtr& msg) {
    landmarkInfoMsg = *msg;
}

void IRCallback(const lab2::range::ConstPtr& msg) {
    IRMsg = *msg;
}

void arduinoCallback(const waypoint::arduinoAngle::ConstPtr& msg) {
	arduinoInfoMsg = *msg;
}


//TODO: camera servo callback
/*
void IMUCallback(const waypoint::camera?::ConstPtr& msg) {
}
*/


int main(int argc, char **argv) {

    ros::init(argc, argv, "fanboat_info_node");

    ros::NodeHandle n;


    double tgtCD;
    double tgtID;
    
    n.getParam("target_cam_distance", tgtCD);
    n.getParam("target_IR_distance", tgtID);
 
    fbInfo.tgtCamDistance = (float) tgtCD;
    fbInfo.tgtIrDistance = (float) tgtID;
 
    //TODO: get rid of this for non-debugging
    fbInfo.tgtLandmark = 68;

    ros::Publisher infoPub = n.advertise<waypoint::fanboatInfo>("/fanboatInfo", 1000);
    
    ros::Subscriber controlSub = n.subscribe("/fanboat_control", 1000, controlCallback);
    
    ros::Subscriber tgtLandmarkSub = n.subscribe("/waypoint/landmark", 1000, tgtLandmarkCallback);
    
    ros::Subscriber IMUSub = n.subscribe("/fanboatLL", 1000, IMUCallback);
    
    ros::Subscriber motorSub = n.subscribe("/motors", 1000, motorCallback);
    ros::Subscriber landmarkSub = n.subscribe("/landmarkInfo", 1000, landmarkCallback);
    
    ros::Subscriber IRSub = n.subscribe("/ir_range", 1000, IRCallback);

	ros::Subscriber arduino = n.subscribe("servoAngle", 1000, arduinoCallback);
    
    
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
        
        //TODO: get all of these
           
        fbInfo.tgtCamDistance; //this comes from launch
        fbInfo.tgtIrDistance;  //this comes from launch
        
        fbInfo.curLandmark = landmarkInfoMsg.id;
        fbInfo.curCamDistance = landmarkInfoMsg.distance;
        
        //TODO: These might be backwards. Double check IR a and b vs L and R
        fbInfo.IRLDist = IRMsg.a;
        fbInfo.IRRDist = IRMsg.b;

		//Arduino info 
		fbInfo.camAngle = arduinoInfoMsg.camAngle;
		fbInfo.camCentered = arduinoInfoMsg.centered;
        
        //fbInfo.camAngle
        
        infoPub.publish(fbInfo);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
