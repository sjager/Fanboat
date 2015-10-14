#include <ros/ros.h>
#include <lab2/range.h>
#include <lab2/magnitude.h>
#include <lab2/angle.h>
#include <fanboat_ll/fanboatLL.h>
#include <algorithm>

lab2::range rangeMsg;
lab2::angle angleMsg;
lab2::magnitude magMsg;
fanboat_ll::fanboatLL IMUMsg;
double target_dist;

float max_dist = 1.5;
int mapping_type = 0;
float rangeA;
float rangeB;
int flag = 0;

int ang;
float dist;
float exDist = 0.0;
if(mapping_type == 0.0){exDist = 100.0;}
int exAng = 0;
float startAngle;

void controllerCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  
  int input = INVALID_INPUT;
  
  int yDown = msg->buttons[3];
  
  if(yDown == 1) {startAngle = IMUMsg.yaw;}

}

void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
    ROS_INFO("IMU.yaw (callback):%f", msg->yaw);  
    IMUMsg = *msg;
}

void inputCallback(const lab2::range::ConstPtr& msg)
{
    ROS_INFO("IR callback");
	//set globals for the current ranges
	rangeA = rangeMsg.a;
	rangeB = rangeMsg.b;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mapping_node");

    ros::NodeHandle n;

    ros::Publisher mag_pub = n.advertise<lab2::magnitude>("/mapping_magnitude", 1000);
    ros::Publisher ang_pub = n.advertise<lab2::angle>("/mapping_angle", 1000);
    ros::Subscriber sub = n.subscribe("/ir_range", 1000, inputCallback);
	ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
	ros::Subscriber sub = n.subscribe("/joy", 1000, controllerCallback);
	
    n.getParam("mapping_type", mapping_type);
    
    ROS_INFO("flag = %i", flag);
	
	if(IMUMsg.yaw - startAngle == 180) {flag = 1;}

	if(flag == 0){
        ROS_INFO("about to spin");
		ROS_INFO("IMUsg.yaw= %f", IMUMsg.yaw);
  		angleMsg.angle = IMUMsg.yaw + 90; //for statement had i, not 45
		ROS_INFO("I'm trying to get to the angle: %f\n", angleMsg.angle);
		//get the current angle and distance
		ang = IMUMsg.yaw - 360.0;
		//record the new shortest/longest distance
		if(mapping_type == 0) {
			dist = std::min(rangeA, rangeB);
			if(dist < exDist) {
				ROS_INFO("New nearest found: %f, %f", dist, ang);
				exDist = dist;
				exAng = ang;
			}
		} else {
			dist = std::max(rangeA, rangeB);
			if(dist > exDist) {
				ROS_INFO("New furthest found: %f, %f", dist, ang);
				exDist = dist;
				exAng = ang;
			}
		}
	}

	if(flag == 1){angleMsg.angle = exAng;}

    ros::Rate loop_rate(8);

    while(ros::ok())
    {
        mag_pub.publish(magMsg);
        ang_pub.publish(angleMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}

