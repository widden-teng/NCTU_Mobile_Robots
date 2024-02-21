#include "ros/ros.h"
#include "std_msgs/Char.h"

std_msgs::Char flag;

void Callback(const std_msgs::Char::ConstPtr &in_value)
{ 
  if(flag.data!=in_value->data){
    ROS_INFO("I heard: [%c]", in_value->data);
    flag.data=in_value->data;
  }
  
}

int main(int argc, char **argv)
{
  //flag.data=0;
  ros::init(argc, argv, "input");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("a2pc", 1000, Callback);

  ros::spin();

  return 0;
}
