#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "motor_serial/motor_serial.h"

double vel_x, vel_y, yaw_angle;

void velCallback2 (const geometry_msgs::Twist& vel) {
  vel_x = vel.linear.x;
  vel_y = vel.linear.y;
  yaw_angle = vel.angular.z;
}

int main (int argc, char **argv) {
  int id_num[4] = {0,1,2,3};
  int cmd_num[4] = {0,1,2,3};
  motor_serial::motor_serial srv;
  double vel[4];
  double a = 100, b = 100;//a,bはそれぞれロボットの横幅,縦幅
  ros::init (argc, argv, "mechanum");
  ros::NodeHandle n;
  ros::Subscriber vel_sub_2 = n.subscribe ("/cmd_vel", 10, velCallback2);
  ros::ServiceClient motor_speed = n.serviceClient <motor_serial::motor_serial> ("motor_info");
  ros::Rate loop_rate(500);

  while (ros::ok()) {
    vel[0] = -1*vel_x + vel_y + (a/2+b/2)*yaw_angle;
    vel[1] = 1*vel_x + vel_y - (a/2+b/2)*yaw_angle;
    vel[2] = -1*vel_x + vel_y - (a/2+b/2)*yaw_angle;
    vel[3] = 1*vel_x + vel_y + (a/2+b/2)*yaw_angle;

    for (int i = 0; i < 4; ++i) {
      srv.request.id = id_num[i];
      srv.request.cmd = cmd_num[i];
      srv.request.data = static_cast<int>(vel[i]);
      motor_speed.call(srv);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
