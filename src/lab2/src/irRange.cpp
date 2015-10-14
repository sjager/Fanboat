#include <ros/ros.h>
#include <fanboat_ll/fanboatLL.h>
#include <lab2/range.h>
#include <math.h>

fanboat_ll::fanboatLL fanboatMsg;
lab2::range rangeMsg;

float calc_range(float volt)
{
    //y=245.09*x^(-1.129)
    float range = 245.09 * pow(volt, -1.129);
    return range;
}

void inputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) 
{

    float volt_A = msg->a2;
    float volt_B = msg->a3;

    float range_A = calc_range(volt_A);
    float range_B = calc_range(volt_B);

    rangeMsg.header = fanboatMsg.header;

    rangeMsg.a = range_A;
    rangeMsg.b = range_B;
}

int main(int argc, char **argv) 
{
    ros::init(argc, argv, "ir_range_node");

    ros::NodeHandle n;

    ros::Publisher pub = n.advertise<lab2::range>("/ir_range", 1000);
    ros::Subscriber sub = n.subscribe("/fanboatLL", 1000, inputCallback);

    ros::Rate loop_rate(8);

    while(ros::ok())
    {
        pub.publish(rangeMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
