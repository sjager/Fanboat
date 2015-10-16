#include <ros/ros.h>
#include <lab2/range.h>
#include <lab2/magnitude.h>
#include <lab2/angle.h>
#include <lab2/toggle.h>

lab2::angle angleMsg;
lab2::magnitude magMsg;
lab2::toggle toggleMsg;
double target_dist;

float max_dist = 1.0;

void inputCallback(const lab2::range::ConstPtr& rangeMsg)
{
    ROS_INFO("Range A: %f\n", rangeMsg->a);
    ROS_INFO("Range B: %f\n", rangeMsg->b);

    toggleMsg.toggle = false;

    //range.a is the portside, range.b is starboard
    //If both sensors see a distance furthar than our target, go forward. 
    if( (rangeMsg->a > target_dist) || (rangeMsg->b > target_dist) )
    {
        magMsg.magnitude = .7;
    }
    else
    {
        magMsg.magnitude = 0;
    }

    if( (rangeMsg->a > max_dist) && (rangeMsg->b > max_dist))
    {
        magMsg.magnitude = 0;
        toggleMsg.toggle = true;
    }

    ROS_INFO("Magnitude: %f\n", magMsg.magnitude);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "reactive_control_node");

    ros::NodeHandle n;
    
    ros::Publisher mag_pub = n.advertise<lab2::magnitude>("/reactive_magnitude", 1000);
    ros::Publisher ang_pub = n.advertise<lab2::angle>("/reactive_angle", 1000);
    ros::Publisher toggle_pub = n.advertise<lab2::toggle>("/react_switch", 1000);
    ros::Subscriber sub = n.subscribe("/ir_range", 1000, inputCallback);

    ros::Rate loop_rate(8);

    n.getParam("target_dist", target_dist);

    while(ros::ok())
    {
        mag_pub.publish(magMsg);
        ang_pub.publish(angleMsg);
        toggle_pub.publish(toggleMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
