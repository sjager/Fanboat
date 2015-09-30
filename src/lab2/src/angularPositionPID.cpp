#include <ros/ros.h>
#include <lab2/angle.h>
#include <fanboat_ll/fanboatLL.h>
#include <fanboat_ll/fanboatMotors.h>

fanboat_ll::fanboatMotors pubMsg;

fanboat_ll::fanboatLL prevInputMsg;
fanboat_ll::fanboatLL curInputMsg;

const float BAND_WIDTH = 0.12;
const float DEFAULT_POWER = 0.12;

ros::NodeHandle n;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void imuInputCallback(const fanboat_ll::fanboatLL::ConstPtr& msg) {
  prevInputMsg = curInputMsg;
  curInputMsg = *msg;
}

void angleInputCallback(const lab2::angle::ConstPtr& msg) {
  if(prevInputMsg.yaw != 0.0) {
    ROS_INFO("I heard: a message");
    pubMsg.header = msg->header;

    // zigbeee initializes yaw to 360
    float currentDir = curInputMsg.yaw - 360.0;
    long currentTS = curInputMsg.header.stamp.sec + curInputMsg.header.stamp.nsec / 1000000000;
    float previousDir = prevInputMsg.yaw - 360.0;
    long prevTS = prevInputMsg.header.stamp.sec + prevInputMsg.header.stamp.nsec / 1000000000;

    float targetAngle = msg->angle;
    // Find error from target (potentially switch??)
    float degreesToTurn = targetAngle - currentDir;
    float prevDegreesToTurn = targetAngle - previousDir;

    // Parse degrees to less than 180 degree turns
    if(degreesToTurn > 180.0) {
      degreesToTurn = (degreesToTurn - 360.0) * -1.0;
      prevDegreesToTurn = (prevDegreesToTurn - 360.0) * -1.0;
    } else if (degreesToTurn < -180.0) {
      degreesToTurn = (degreesToTurn + 360.0) * -1.0; 
      prevDegreesToTurn = (prevDegreesToTurn + 360.0) * -1.0;
    }

    double pParam = 0.0;
    n.getParam("pParam", pParam);
    double dParam = 0.0;
    n.getParam("dParam", dParam);

    float mot_r = 0.0;
    float mot_l = 0.0;

    if(degreesToTurn > 0.0) {
      //turn right?
      mot_r = .12;
      mot_l = pParam * degreesToTurn + dParam * (degreesToTurn - prevDegreesToTurn) / (currentTS - prevTS);
    } else {
      //turn left?
      mot_l = .12;
      mot_r = pParam * degreesToTurn + dParam * (degreesToTurn - prevDegreesToTurn) / (currentTS - prevTS);
    }

    if(mot_r > 1.0) mot_r = 1.0;
    if(mot_l > 1.0) mot_l = 1.0;
    if(mot_r < 0.12) mot_r = 0.12;
    if(mot_l < 0.12) mot_l = 0.12;

    ROS_INFO("Right Motor: %5f Left Motor: %5f",mot_r, mot_l);

    pubMsg.right = mot_r;
    pubMsg.left = mot_l;
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "nurse_joy_node");

  ros::Publisher pub = n.advertise<fanboat_ll::fanboatMotors>("/motors", 1000);
  ros::Subscriber anglesub = n.subscribe("/target_angle", 1000, angleInputCallback);
  ros::Subscriber imusub = n.subscribe("/fanboatLL", 1000, imuInputCallback);
  ros::Rate loop_rate(8);

  pubMsg.right = .12;
  pubMsg.left = .12;

  while(ros::ok()) {
    pub.publish(pubMsg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
