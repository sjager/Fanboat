#include <ros/ros.h>
#include <lab2/range.h>
#include <lab2/magnitude.h>
#include <lab2/angle.h>
#include <lab2/toggle.h>
#include <fanboat_ll/fanboatLL.h>
#include <algorithm>
#include <sensor_msgs/Joy.h>

#define BUFFER_SIZE 10
#define ANGLE_TOL_START -5.0
#define ANGLE_TOL_END -15.0 
#define TURN_SPEED 30

lab2::range rangeMsg;
lab2::angle angleMsg;
lab2::magnitude magMsg;
lab2::toggle toggleMsg;
fanboat_ll::fanboatLL IMUMsg;

// to filter out noise, we are using running averages of n=BUFFER_SIZE values.
float minBuffer[BUFFER_SIZE];
float maxBuffer[BUFFER_SIZE];

int mapping_type = 0;

float rangeA;
float rangeB;

// determines if the robot should be spinning or seeking the best angle.
// the robot will continue to spin while flag =0. When it has spun close to 360 degrees,
// flag will be updated to 1.
int flag = 0;

//current angle and distance
float ang;
float dist = 999.0;

float minDist = 999.0;
float maxDist = -1.0;
float minAng = 0.0;
float maxAng = 0.0;

// the angle that should be used as a reference for detecting a full spin.
float startAngle;

float averageMinDist = 999.0;
float averageMaxDist = -1.0;

// updates the buffer of min values and updates the average.
void insertIntoMinBuffer(float newVal) {
  
  for(int i = 0; i < BUFFER_SIZE - 1; i++) {
    minBuffer[i] = minBuffer[i+1];
  }
  minBuffer[BUFFER_SIZE-1] = newVal;
  
  float runningSum = 0;
  
  for(int j = 0; j < BUFFER_SIZE; j++) {
    runningSum += minBuffer[j];
  }
  averageMinDist = runningSum / BUFFER_SIZE;
  
}

// updates the buffer of max values and updates the average.
void insertIntoMaxBuffer(float newVal) {

  for(int i = 0; i < BUFFER_SIZE - 1; i++) {
    maxBuffer[i] = maxBuffer[i+1];
  }
  maxBuffer[BUFFER_SIZE-1] = newVal;
  
  float runningSum = 0;
  
  for(int j = 0; j < BUFFER_SIZE; j++) {
    runningSum += maxBuffer[j];
  }
  averageMaxDist = runningSum / BUFFER_SIZE;
  
}


void controllerCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  
  int yDown = msg->buttons[3];
  
  // when the Y button is pressed, it restarts the node;
  if(yDown == 1) {
    startAngle = IMUMsg.yaw;  
    ROS_INFO("\n\n\nRESET\n\n\n");
  }
  
  // reset the flag so the robot restarts the circle
  flag = 0;
  
  // reset best values
  minDist = 9999.0;
  minAng = 0.0;
  maxDist = -1.0;
  maxAng = 0.0;
  
  // reset buffer values
  for(int i = 0; i < BUFFER_SIZE; i++) {
    minBuffer[i] = 9999.9;
    maxBuffer[i] = -1.0;
  }
  
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
	
	//update minimum distances
	dist = std::min(rangeA, rangeB);
	if(dist == 0.0) //when the node first starts, the IR sensors report 0.0 until they are fully initialized. We throw these values out.
	{
	  dist = 500;
	}
	insertIntoMinBuffer(dist);
	
	//update maximum distances
	dist = std::max(rangeA, rangeB);
	insertIntoMaxBuffer(dist);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mapping_node");

    ros::NodeHandle n;

    ros::Publisher mag_pub = n.advertise<lab2::magnitude>("/mapping_magnitude", 1000);
    ros::Publisher ang_pub = n.advertise<lab2::angle>("/mapping_angle", 1000);
    ros::Publisher toggle_pub = n.advertise<lab2::toggle>("/map_switch", 1000); 
    
    ros::Subscriber sub = n.subscribe("/ir_range", 1000, inputCallback);
	  ros::Subscriber fanboat = n.subscribe("/fanboatLL", 1000, IMUinputCallback);
	  ros::Subscriber joysub = n.subscribe("/joy", 1000, controllerCallback);
	
    n.getParam("mapping_type", mapping_type);
    
    ros::Rate loop_rate(8);

    while(ros::ok())
    {
      float angleDif = IMUMsg.yaw - startAngle;

      toggleMsg.toggle = false;  
      
      // if the robot detects an angle difference that falls within a certain range, we can assume it has turned a full circle.    
      if((angleDif < ANGLE_TOL_START) && (angleDif > ANGLE_TOL_END)) {
        flag = 1;
      }

      if(flag == 0) {
        ROS_INFO("CUR: %f, ST: %f, DIF: %f, DIST: %f, BDIST: %F, BANG: %f",
         IMUMsg.yaw, startAngle, (IMUMsg.yaw - startAngle), averageMinDist, minDist, minAng);
	      
	      // rotate the boat
	      angleMsg.angle = IMUMsg.yaw + TURN_SPEED; 
        
        //get the current angle and distance
        ang = IMUMsg.yaw - 360.0;
        
        //record the new shortest
        if((dist != 0.0)&&(averageMinDist < minDist)) {
	        ROS_INFO("New shortest found: D: %f, ANG: %f", averageMinDist, ang);
	        minDist = averageMinDist;
	        minAng = ang;
        }
        
        //record the new furthest
        if(averageMaxDist > maxDist) {
	        ROS_INFO("New furthest found: D: %f, ANG: %f", averageMaxDist, ang);
	        maxDist = averageMaxDist;
	        maxAng = ang;
        }
      }

      float bestAngle = 0.0;
      float bestDistance = -1.0;

      //if flag ==1, then the robot should stop spinning and begin to turn to the best angle.
      if(flag == 1) {
        // change the target angle depending on if we're looking for the closest or furthest object
        if(mapping_type == 0) {
          bestAngle = minAng;
          bestDistance = minDist;
        } else {
          bestAngle = maxAng;
          bestDistance = maxDist;
        }
       
        angleMsg.angle = bestAngle - 45.0;
        ROS_INFO("CUR: %f, SK: %f, DIF: %f, BDIST: %f", IMUMsg.yaw, bestAngle, (IMUMsg.yaw - bestAngle), bestDistance);
        angleDif = IMUMsg.yaw - bestAngle;

        // if the robot detects an angle difference that falls within a certain range, we can assume it has turned towards the best angle  
        ROS_INFO("Here is the difference of our best angle! %f", angleDif);  
        if(fmod(fabs(angleDif), 360.0) < 40.0) {
          toggleMsg.toggle = true;
        }        
      }

      mag_pub.publish(magMsg);
      ang_pub.publish(angleMsg);
      toggle_pub.publish(toggleMsg);      

      ros::spinOnce();
      loop_rate.sleep();
    }
    
    return 0;
}

