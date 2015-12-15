#include <ctime>
#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <lab3/fanboatControl.h>
#include <lab3/metrics.h>
#include <waypoint/landmarkInfo.h>
#define ARRAYSIZE 10

using landmark_self_sim::landmarkLocation;
using waypoint::landmarkInfo;

int histArray[ARRAYSIZE];
int currentIndex = -1;

int arraySize;
double threshold;

double fillPercentage = 0.0;

landmarkLocation location;

landmarkInfo info;

double distance;

int seq_loc=0;
int seq_info=0;


void instantiateArray() {
    int i;
    for(i = 0; i< ARRAYSIZE; i++) {
        histArray[i] = 0;
    }
}

void updateArray() {
    int val = 0;
    
    ROS_INFO("seq_loc: %d, seq_info %d", seq_loc, seq_info);

    if(seq_loc>seq_info) {
        fillPercentage += 0.1;
    } else {
        fillPercentage -= 0.1;
    }

    ROS_INFO("Fill Percentage: %f", fillPercentage);

    if(fillPercentage < 0) {fillPercentage = 0.0;}
    if(fillPercentage > 1) {fillPercentage = 1.0;}
}

float calculateDistance(float height) {
  float dist = 145.86*pow(height, -0.993);
  return dist;
}


void locationCallback(const landmarkLocation::ConstPtr& msg) {
    location = *msg;

    distance = calculateDistance(location.height);

    seq_loc++;
    
}


int main(int argc, char **argv) {

    instantiateArray();

    ros::init(argc, argv, "landmark_info_node");

    ros::NodeHandle n;
    
    n.getParam("threshold", threshold);

    ros::Publisher infoPub = n.advertise<waypoint::landmarkInfo>("/landmarkInfo", 1000);

    ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, locationCallback);

    ros::Rate loop_rate(8);

    while(ros::ok()) {

        updateArray();
        /*
        if(seq_loc>seq_info)
        {
            info.header = location.header;
            info.id = location.code;
            info.distance = distance;

            seq_info = seq_loc;
        }
        else
        {
            info.header = location.header;
            info.id = 0;
            info.distance = 9999;   
        }
        */
        ROS_INFO("Fill: %f perc.", fillPercentage);
        if(fillPercentage >= threshold) {
            info.header = location.header;
            info.id = location.code;
            info.distance = distance;

            seq_info = seq_loc;
            ROS_INFO("Landmark!");
        } else {
            info.header = location.header;
            info.id = 0;
            info.distance = 9999;
            ROS_INFO("NOTHING!");
        }

        infoPub.publish(info);
        
        //ROS_INFO("Sequence Info %i", seq_info);
        //ROS_INFO("Sequence Location %i", seq_loc);       

        ros::spinOnce();
        loop_rate.sleep();
    }

}
