#include <ros/ros.h>
#include <lab2/range.h>
#include <lab2/magnitude.h>
#include <lab2/angle.h>
#include <fanboat_ll/fanboatLL.h>
#include <algorithm>

lab2::range rangeMsg;
lab2::angle angleMsg;
lab2::magnitude magMsg;
fanboat_ll:: fanboatLL IMUMsg;
double target_dist;

float max_dist = 1.5;
int mapping_type = 0;
float rangeA;
float rangeB;
int flag = 0;


void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
  IMUMsg = *msg;
}

void inputCallback(const lab2::range::ConstPtr& msg)
{
	//set globals for the current ranges
	rangeA = rangeMsg.a;
	rangeB = rangeMsg.b;
}

void spin() {
//set up the variables needed
	int ang;
	float dist;
	float exDist = 0.0;
	if(mapping_type == 0.0){exDist = 100.0;}
	int exAng = 0;
	float startAngle = IMUMsg.yaw;
    
	//loop through the whole circle
	//for(int i = 0; i < 360; i += 10) { //If use for, update next line too
	while(fabs(startAngle) - fabs(IMUMsg.yaw) > 10) {
		//tell the boat to move a bit further than it currently is
		//does angleMsg need a header?
  		angleMsg.angle = IMUMsg.yaw + 90; //for statement had i, not 45
		ROS_INFO("I'm trying to get to the angle: %f\n", angleMsg.angle);
		//get the current angle and distance
		ang = IMUMsg.yaw - 360.0;
		//record the new shortest/longest distance
		if(mapping_type == 0) {
			dist = std::min(rangeA, rangeB);
			if(dist < exDist) {
				exDist = dist;
				exAng = ang;
			}
		} else {
			dist = std::max(rangeA, rangeB);
			if(dist > exDist) {
				exDist = dist;
				exAng = ang;
			}
		}
	}

	angleMsg.angle = exAng;
	return;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mapping_node");

    ros::NodeHandle n;

    ros::Publisher mag_pub = n.advertise<lab2::magnitude>("/mapping_magnitude", 1000);
    ros::Publisher ang_pub = n.advertise<lab2::angle>("/mapping_angle", 1000);
    ros::Subscriber sub = n.subscribe("/ir_range", 1000, inputCallback);
	ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
	
	while(flag == 0){
		spin();
		flag = 1;
	}

    ros::Rate loop_rate(8);

    n.getParam("mapping_type", mapping_type);

    while(ros::ok())
    {
        mag_pub.publish(magMsg);
        ang_pub.publish(angleMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}

