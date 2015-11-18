#include <ros/ros.h>
#include <string.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>
#include <landmark_self_sim/landmarkLocation.h>
#include <lab3/fanboatControl.h>
#include <lab2/range.h>

#define THRESH  0.3

lab2::range irMsg;

void irCallback(const lab2::range::ConstPtr& msg)
{
    irMsg = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "collection_switch_node");
    
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("/ir_range", 1000, irCallback);

    ros::Rate loop_rate(15);

    while(ros::ok())
    {
        ROS_INFO("A: %f  B: %f", irMsg.a, irMsg.b);
        if(irMsg.a > THRESH || irMsg.b > THRESH)
        {
            ROS_INFO("BALL");
            
            //We have captured a ball, so switch from ball detection to landmark
            n.setParam("detectBall", 0);
            n.setParam("detectLandmark", 1);
        }
        else
        {
            ROS_INFO("no ball");
            n.setParam("detectBall", 1);
            n.setParam("detectLandmark", 0);
        }
        
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
