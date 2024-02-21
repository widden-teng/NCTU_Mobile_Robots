#include "ros/ros.h"
#include <wiringPi.h>
#include <iostream>
#include <std_msgs/Int32.h>

//light receive pin 3
int lightpin = 3;

int main (int argc, char **argv){

    ros::init(argc, argv, "light");
    ros::NodeHandle n;
    ros::Publisher light_pub = n.advertise<std_msgs::Int32>("pc2a", 10);
    
    wiringPiSetup();

    int light_rev = 0;
    std_msgs::Int32 light_data;

    setenv("WIRINGPI_GPIOMEM", "1", 1);
    pinMode(lightpin, INPUT);
    
    ros::Rate loop_rate(10);

    while(ros::ok()){
        light_rev = digitalRead(lightpin) ;
        light_data.data = light_rev;
        ROS_INFO("light_receive : %d ", light_rev);
        light_pub.publish(light_data);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0 ;
}

