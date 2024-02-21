#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <sstream>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "output");

  ros::NodeHandle n;
  ros::Publisher value_pub = n.advertise<std_msgs::Int32>("pc2a", 100);

  // ros::Rate loop_rate(10);

  std_msgs::Int32 put_value;
  put_value.data=0;
  value_pub.publish(put_value);
  int value = 0;
  while (ros::ok())
  {
    std::cin>>value;
    put_value.data = value;

    ROS_INFO("%d", put_value.data);

    value_pub.publish(put_value);
    // ros::spinOnce();
  }

  return 0;
}