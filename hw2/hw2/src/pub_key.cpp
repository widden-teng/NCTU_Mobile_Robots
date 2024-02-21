#include "ros/ros.h"
#include "std_msgs/Char.h"
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{

  ros::init(argc, argv, "output");

  ros::NodeHandle n;
  ros::Publisher value_pub = n.advertise<std_msgs::Char>("pc2a", 100);

  std_msgs::Char put_value;
  char value;
  while (ros::ok())
  {
  	cout << "Control mode: ";
   	cin >> value;
   	put_value.data = value;

   	value_pub.publish(put_value);
  }

  return 0;
}
