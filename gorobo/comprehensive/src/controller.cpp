#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

double tent_vel_x, tent_vel_y, tent_vel_z;

void vellCallback (const sensor_msgs::Joy &a) {
  tent_vel_x = a.axes[1];
  tent_vel_y = a.axes[0];
  tent_vel_z = a.axes[2];
}

int main (int argc, char **argv) {
  geometry_msgs::Twist vel;
  ros::init (argc, argv, "controller");
  ros::NodeHandle n;
  ros::Subscriber vel_sub = n.subscribe ("joy", 10, vellCallback);
  ros::Publisher vel_pub  = n.advertise<geometry_msgs::Twist> ("/cmd_vel", 10);
  ros::Rate loop_rate(500);

  while (ros::ok()) {
    vel.linear.x = 1.5 * tent_vel_x;
    vel.linear.y = 1.5 * tent_vel_y;
    vel.angular.z = 1.5 * tent_vel_z;
    vel_pub.publish(vel);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
