#include <ctime>
#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <lab3/fanboatControl.h>
#include <lab3/metrics.h>
#include <waypoint/landmarkInfo.h>

using landmark_self_sim::landmarkLocation;
using waypoint::landmarkInfo;

landmarkLocation location;

landmarkInfo info;

double distance;

int seq_loc=0;
int seq_info=0;


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

    ros::init(argc, argv, "landmark_info_node");

    ros::NodeHandle n;

    ros::Publisher infoPub = n.advertise<waypoint::landmarkInfo>("/landmarkInfo", 1000);

    ros::Subscriber landmarkSub = n.subscribe("/landmarkLocation", 1000, locationCallback);

    ros::Rate loop_rate(8);

    while(ros::ok()) {

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
            info.distance = -1;   
        }


        infoPub.publish(info);
        
        //ROS_INFO("Sequence Info %i", seq_info);
        //ROS_INFO("Sequence Location %i", seq_loc);       

        ros::spinOnce();
        loop_rate.sleep();
    }

}
