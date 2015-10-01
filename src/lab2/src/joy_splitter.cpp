#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <lab2/angle.h>
#include <lab2/magnitude.h>
#include <math.h>

lab2::angle anglePubMsg;
lab2::magnitude magPubMsg;

float prevAngle = 0.0;

const float DEAD_ZONE_MAGNITUDE = 0.12;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void inputCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  
  ROS_INFO("Y: %f, X: %f\n", msg->axes[1], msg->axes[0]);
  
  float y = msg->axes[1];
  float x = -1.0 * msg->axes[0]; //times -1 to make a normal unit circle
  
  float angle = atan2(y,x); //angle is in radians at this point
  angle = angle * 180.0 / 3.141592654;
  
  float magnitude = sqrt(pow(x,2) + pow(y,2));
  
  // rotate the angle reading so that 0 is forward and 90 is right
  if((angle <=-90) && (angle >= -180))
  {
    angle = -1.0 * angle - 270;
  }
  else
  {
    angle = -1.0 * angle + 90;
  }
  
  anglePubMsg.header = msg->header;
  anglePubMsg.angle = angle;

  magPubMsg.header = msg->header;
  magPubMsg.magnitude = magnitude;
  
  if((fabs(y) <= DEAD_ZONE_MAGNITUDE) && (fabs(x) <= DEAD_ZONE_MAGNITUDE))
  {
    ROS_INFO("DEADZONE, y:%f, x:%f",fabs(y), fabs(x));
    anglePubMsg.angle = prevAngle;
    magPubMsg.magnitude = 0;
  } else {
    prevAngle = angle;
  }
  
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "joy_splitter_node");
  
  ros::NodeHandle n;
  
  
  ros::Publisher anglePub = n.advertise<lab2::angle>("/target_angle", 1000);
  ros::Publisher magnitudePub = n.advertise<lab2::magnitude>("/target_magnitude", 1000);
  ros::Subscriber sub = n.subscribe("/joy", 1000, inputCallback);

  ros::Rate loop_rate(13);

  while(ros::ok()) {
    anglePub.publish(anglePubMsg);
    magnitudePub.publish(magPubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
