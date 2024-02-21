#include <ros.h>
#include <std_msgs/Int32.h>

ros::NodeHandle  nh;

std_msgs::Int32 output;

void callback(const std_msgs::Int32 &input){ 
  output.data=input.data*2;
}
ros::Publisher ros_pub("a2pc", &output);
ros::Subscriber<std_msgs::Int32> ros_sub("pc2a", &callback );

void setup()
{
  nh.initNode();
  nh.advertise(ros_pub);
  nh.subscribe(ros_sub);
}

void loop()
{ 
  
  nh.spinOnce();
  delay(1);
  ros_pub.publish(&output);
}
