#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <lab3/fanboatControl.h>
#include <lab2/range.h>
#include <lab3/fanboatControl.h>

#define THRESH  0.3

lab2::range irMsg;
lab3::fanboatControl controlMsg;

bool ballMode = true;
bool landmarkMode = false;
bool latch = false;

void irCallback(const lab2::range::ConstPtr& msg)
{
    irMsg = *msg;
}

void ballCallback(const lab3::fanboatControl::ConstPtr& msg)
{
    if(ballMode && !latch)
    {
      controlMsg = *msg;
    }
    
}

void landCallback(const lab3::fanboatControl::ConstPtr& msg)
{
    if(landmarkMode || latch)
    {
      controlMsg = *msg;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "collection_switch_node");
    
    ros::NodeHandle n;

    ros::Subscriber irSub = n.subscribe("/ir_range", 1000, irCallback);
    ros::Subscriber ballSub = n.subscribe("/follow/ball", 1000, ballCallback);
    ros::Subscriber landSub = n.subscribe("/follow/landmark", 1000, landCallback);
    ros::Publisher controlPub = n.advertise<lab3::fanboatControl>("/fanboat_control", 1000);

    ros::Rate loop_rate(15);

    while(ros::ok())
    {
        ROS_INFO("A: %f  B: %f", irMsg.a, irMsg.b);
        if(irMsg.a > THRESH || irMsg.b > THRESH)
        {
            ROS_INFO("BALL!!!!!");
            
            //We have captured a ball, so switch from ball detection to landmark
            n.setParam("detectBall", 0);
            n.setParam("detectLandmark", 1);
            
            landmarkMode = true;
            ballMode = false;
            latch = true;
        }
        else
        {
            ROS_INFO("no ball !!!!!!");
            n.setParam("detectBall", 1);
            n.setParam("detectLandmark", 0);
            
            ballMode = true;
            landmarkMode = false;
        }

        controlPub.publish(controlMsg);
        
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
