#include <ros/ros.h>
#include <lab2/range.h>
#include <lab2/magnitude.h>
#include <lab2/angle.h>
#include <fanboat_ll/fanboatLL.h>
#include <algorithm>
#include <sensor_msgs/Joy.h>

#define BUFFER_SIZE 10

lab2::range rangeMsg;
lab2::angle angleMsg;
lab2::magnitude magMsg;
fanboat_ll::fanboatLL IMUMsg;
double target_dist;

float buffer[BUFFER_SIZE];

float max_dist = 1.5;
int mapping_type = 0;
float rangeA;
float rangeB;
int flag = 0;

float ang;
float dist = 999.0;
float exDist = 9999.0;

float exAng = 0;
float startAngle;

float averageDist = 999.0;

void insertIntoBuffer(float newVal) {
  for(int i = 0; i < BUFFER_SIZE - 1; i++) {
    buffer[i] = buffer[i+1];
  }
  buffer[BUFFER_SIZE-1] = newVal;
  
  float runningSum = 0;
  
  for(int j = 0; j < BUFFER_SIZE; j++) {
    runningSum += buffer[j];
  }
  
  averageDist = runningSum / BUFFER_SIZE;
  
}

void controllerCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  
  int yDown = msg->buttons[3];
  
  if(yDown == 1) {
    startAngle = IMUMsg.yaw;  
    ROS_INFO("\n\n\nRESET\n\n\n");
  }
  flag = 0;
  exDist = 9999.0;
  exAng = 0.0;
}

void IMUinputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
    //ROS_INFO("IMU.yaw (callback):%f", msg->yaw);  
    IMUMsg = *msg;
}

void inputCallback(const lab2::range::ConstPtr& msg)
{
  rangeMsg = *msg;
	//set globals for the current ranges
	rangeA = rangeMsg.a;
	rangeB = rangeMsg.b;
	dist = std::min(rangeA, rangeB);
	
	if(dist == 0.0)
	{
	  dist = 500;
	}
	
	insertIntoBuffer(dist);
	
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mapping_node");

    ros::NodeHandle n;

    ros::Publisher mag_pub = n.advertise<lab2::magnitude>("/mapping_magnitude", 1000);
    ros::Publisher ang_pub = n.advertise<lab2::angle>("/mapping_angle", 1000);
    ros::Subscriber sub = n.subscribe("/ir_range", 1000, inputCallback);
	  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
	  ros::Subscriber joysub = n.subscribe("/joy", 1000, controllerCallback);
	
    n.getParam("mapping_type", mapping_type);
    
    
    ros::Rate loop_rate(8);

    while(ros::ok())
    {
    
	        if((IMUMsg.yaw - startAngle < -5) && (IMUMsg.yaw - startAngle > -15)) {
	          flag = 1;
	        }

	        if(flag == 0){
	          ROS_INFO("CUR: %f, ST: %f, DIF: %f, DIST: %f, BDIST: %F, BANG: %f", IMUMsg.yaw, startAngle, (IMUMsg.yaw - startAngle), averageDist, exDist, exAng);
			      angleMsg.angle = IMUMsg.yaw + 30; //for statement had i, not 45
		        //ROS_INFO("I'm trying to get to the angle: %f\n", angleMsg.angle);
		        //get the current angle and distance
		        ang = IMUMsg.yaw - 360.0;
		        //record the new shortest/longest distance
		        if(mapping_type == 0) {
			        if((dist != 0.0)&&(averageDist < exDist)) {
				        ROS_INFO("New nearest found: D: %f, ANG: %f", averageDist, ang);
				        exDist = averageDist;
				        exAng = ang;
			        }
		        } else {
			        if(averageDist > exDist) {
				        ROS_INFO("New furthest found: %f, %f", dist, ang);
				        exDist = averageDist;
				        exAng = ang;
			        }
		        }
	        }

	        if(flag == 1) {
	          angleMsg.angle = exAng - 45.0;
	          ROS_INFO("CUR: %f, SK: %f, DIF: %f, BDIST: %f", IMUMsg.yaw, exAng, (IMUMsg.yaw - exAng), exDist);
	        }

    
        mag_pub.publish(magMsg);
        ang_pub.publish(angleMsg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}

