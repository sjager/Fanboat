#include <ros/ros.h>
#include <fanboat_ll/fanboatLL.h>
#include <lab2/range.h>
#include <math.h>
#define SPIKE 1.3
#define LENGTH 20


fanboat_ll::fanboatLL fanboatMsg;
lab2::range rangeMsg;

float values[LENGTH];

float runningAverage = 0;

void UpdateArray(float newVal) {
      
  //if(newVal - values[LENGTH-1] < SPIKE) {
    for (int i = 0; i < LENGTH-1; i++) {
      values[i] = values[i+1];
    }
    values[LENGTH-1] = newVal;
  //}
  
  runningAverage = 0;
  
  for(int j = 0; j < LENGTH; j++) {
    runningAverage += values[j];
  }
  
  runningAverage /= LENGTH;
}

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

    ROS_INFO("range_A: %f\n", range_A);
    ROS_INFO("range_B: %f\n", range_B);
    
    UpdateArray(range_B);
    ROS_INFO("Average: %f\n", runningAverage);
    
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

    ros::Rate loop_rate(15);

    while(ros::ok())
    {
        pub.publish(rangeMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
