#include "ros/ros.h"
#include "std_msgs/String.h"
#include <detection_rouge/barycentre_data.h>
#include <std_msgs/Float64MultiArray.h>

float Vx = 0, Vh = 0;
float  P = 0.0015;
float  D = 0.000;
float  I = 0.000;
float  Ph = 0.0018;
float  Dh = 0.000;
float  Ih = 0.000;
int i = 0;
float decalage_x = 0, decalage_height = 0, x_d = 0, y_d = 0, height_d = 0;
ros::Publisher command_pid;

void PIDCallback(const std_msgs::Float64MultiArray::ConstPtr& msg){
	std_msgs::Float64MultiArray myMsg;
	x_d = msg->data[0];
	y_d = msg->data[1];
	height_d = msg->data[2];
	decalage_x=430-x_d;
	decalage_height=400-height_d;
    	Vx = P*decalage_x;
   	Vh = Ph*decalage_height;
  // Saturate values

 if( Vx > 0.2 ){
        Vx = 0.2;
 }
 else if( Vx < -0.2){
        Vx = -0.2;
 }
 else{
	if(Vh > 0.2){
		Vh=0.2;
	}
	else if(Vh<-0.2){
		Vh=-0.2;
	}
 }
		myMsg.data.resize(2);
		myMsg.data[0] = Vx;
		myMsg.data[1] = Vh;
		command_pid.publish(myMsg);
}
/*------------------------------------------  End of callback  ---------------------------------------*/

/*----------------------------------------------  Main  ----------------------------------------------*/
int main(int argc, char** argv)
{
  ros::init(argc, argv, "command_wc");
  ros::NodeHandle n,p;
  ros::Subscriber sub = p.subscribe("/barycentre_decision",1,PIDCallback);
  command_pid = n.advertise<std_msgs::Float64MultiArray>("/command_pid", 1000);
  ros::spin();
  return 0;
}
/*-------------------------------------------  End of Main  ------------------------------------------*/
	
