#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float64.h>
#include <detection_rouge/barycentre_data.h>
 
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

int i=0;
float x_d[1000],y_d[1000],height_d[1000],decalage[1000],x=0,y=0,height=0;
ros::Publisher barycentre_pub;

void BaryCallback(const detection_rouge::barycentre_data::ConstPtr& msg)
{

	std_msgs::Float64MultiArray myMsg;
	i = msg->No;
	for(i=0; i<1000;i++){
		x_d[i] = msg->data[0];
		y_d[i] = msg->data[1];
		height_d[i] = msg->data[2];
		decalage[i]=abs(x_d[i]-430);
		if(decalage[i+1]<decalage[i]){
			x=x_d[i+1];
			y=y_d[i+1];
			height=height_d[i+1];
		}
	}
		myMsg.data.resize(3);
		myMsg.data[0] = x;
		myMsg.data[1] = y;
		myMsg.data[2] = height;
		barycentre_pub.publish(myMsg);
}

int main(int argc, char *argv[]){
    ros::init(argc, argv, "decision");
    ros::NodeHandle n,ns;
    ros::Subscriber barycentre_sub = n.subscribe("/barycentre_data", 1, BaryCallback); //personne
    barycentre_pub = ns.advertise<std_msgs::Float64MultiArray>("/barycentre_decision",1000);
    
    ros::spin();
    return 0;
}
