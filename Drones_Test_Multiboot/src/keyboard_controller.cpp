#include <drone_test/keyboard_controller.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float64.h>
#include <detection_rouge/barycentre_data.h>

float x=0,y=0,height=0,Vx=0,Vh=0;

void BaryCallback(const std_msgs::Float64MultiArray::ConstPtr& msg){
	x=msg->data[0];
	y=msg->data[1];
	height=msg->data[2];
}

void PIDCallback(const std_msgs::Float64MultiArray::ConstPtr& msg){
	Vx=msg->data[0];
	Vh=msg->data[1];
}

// Constructeur
DroneController::DroneController()
{  sub=nh.subscribe("/command_pid", 1, PIDCallback);
   //sub=nh.subscribe("/barycentre_decision", 1, BaryCallback);
   pubTakeoff = nh.advertise<std_msgs::Empty>("/bebop/takeoff", 1000); // On publie sur le topic ardrone/takeoff pour le décollage
   pubLand = nh.advertise<std_msgs::Empty>("/bebop/land", 1000);	// On publie sur le topic ardrone/land pour l'atterissage
   pubEmergency = nh.advertise<std_msgs::Empty>("/bebop/reset", 1000);
   pubPilote = nh.advertise<geometry_msgs::Twist>("/bebop/cmd_vel",1000);
}



// Destructeur
DroneController::~DroneController()
{
  // Il n'y a aucune allocation mémoire, donc pas de désallocation
}

// Fonction permettant de faire décoller l'AR drone
void DroneController::sendTakeoff()
{
   std_msgs::Empty myMsg; 	// on crée notre message qui est de type std_msgs::Empty
   pubTakeoff.publish(myMsg); 	// on le publie sur le topic ardrone/takeoff
}

// Fonction permettant de faire atterir l'AR drone
void DroneController::sendLand()
{
   std_msgs::Empty myMsg;	// on crée notre message qui est de type std_msgs::Empty
   pubLand.publish(myMsg);	// on le publie sur le topic ardrone/land
}

void DroneController::sendEmergency()
{
   std_msgs::Empty myMsg;	// on crée notre message qui est de type std_msgs::Empty
   pubLand.publish(myMsg);
}

void DroneController::sendPilote(float a,float b,float c,float d)
{
   geometry_msgs::Twist myMsg;
   myMsg.linear.x = a;
   myMsg.linear.y = b;
   myMsg.linear.z = c;
   myMsg.angular.x = 0;
   myMsg.angular.y = 0;
   myMsg.angular.z = d;
   pubPilote.publish(myMsg);
}

void DroneController::AutoDetect()
{
    DroneController drone;
    int a=0;
	if(x<360){
		a=0;
	}
	else if(x>510){
	    a=1;
	}
	else{
		a=2;
		if(height>420){
			a=4;
		}
		else if(height<320){
			a=5;
		}
		else{
			a=2;
		}
	}

		  
		
	switch(a){
	case 0:
		drone.sendPilote(0,0,0,0.05);
        break;	

	case 1:
		drone.sendPilote(0,0,0,-0.05);
        break;		

	case 2:
		drone.sendPilote(0,0,0,0);
        break;

	case 3:
		drone.sendPilote(0,0,0,0);
        break;	

	case 4:
		drone.sendPilote(-0.1,0,0,0);
        break;

	case 5:
		drone.sendPilote(0.1,0,0,0);
        break;	

	default:
		drone.sendPilote(0,0,0,0);
        break;
	}  
}

void DroneController::AutoDetectCouleur()
{
    DroneController drone;
    int a=0;
	if(x<350){
		a=0;
	}
	else if(x>500){
	    a=1;
	}
	else{
		a=2;
		if(height>280){
			a=4;
		}
		else if(height<200){
			a=5;
		}
		else{
			a=2;
		}
	}

		  
		
	switch(a){
	case 0:
		drone.sendPilote(0,0,0,0.05);
        break;	

	case 1:
		drone.sendPilote(0,0,0,-0.05);
        break;		

	case 2:
		drone.sendPilote(0,0,0,0);
        break;

	case 3:
		drone.sendPilote(0,0,0,0);
        break;	

	case 4:
		drone.sendPilote(-0.05,0,0,0);
        break;

	case 5:
		drone.sendPilote(0.05,0,0,0);
        break;	

	default:
		drone.sendPilote(0,0,0,0);
        break;
	}  
}
void DroneController::AutoPIDDetect(){
	DroneController drone;
	drone.sendPilote(Vh,0,0,Vx);

}



