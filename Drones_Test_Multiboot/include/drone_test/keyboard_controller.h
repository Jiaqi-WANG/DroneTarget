#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>

class DroneController
{
   public:
      
      DroneController();
      ~DroneController();
      void sendTakeoff();
      void sendLand();
      void sendEmergency();
      void sendPilote(float a,float b,float c,float d);
      void AutoDetect();
      void AutoPIDDetect();
	void AutoDetectCouleur();

   private:
    
      ros::NodeHandle nh,n;
      ros::Subscriber sub;
      ros::Publisher pubTakeoff;
      ros::Publisher pubLand;
      ros::Publisher pubEmergency;
      ros::Publisher pubPilote;
};
