#include <drone_test/keyboard_controller.h>



#include <ncurses.h>  // pour gérer les évènements claviers
#include <unistd.h>   // uniquement pour utiliser la fonction sleep de ncurses

/********************** A LIRE !!!! **********************/
/* Ici on utilise la librairie Ncurses pour gérer les évènements clavier. 
   L'inconvénient avec les méthodes traditionnelles (cin, getchar(), getch(),...)
   est que ces fonctions ATTENDENT que l'on entre une touche, donc si l'on n'entre pas une touche 
   l'AR drone va continuer à avancer en fonction de sa commande précédente, ce n'est pas le comportement voulu,
   on souhaite que l'AR drone avance lorsqu'une touche est appuyée et s'arrête lorsqu'elle est relâchée. 
*/

// Fonction remplaçant un getch() classique (ici il n'y a plus d'attente)

/*void PIDCallback(const std_msgs::Float64::ConstPtr& msg){
    V=msg->data[0];
    Vh=msg->data[1];
    
	
}*/

int kbhit(void)
{
    int ch = getch(); // on demande à l'utilisateur d'entrer une touche

    if (ch != ERR) {  // Si une touche est pressée
	ungetch(ch);
        return 1; 	// on retourne 1
    } else {
        return 0;	// sinon on retourne 0
    }
}

int main(int argc, char **argv)
{

  


  // Initialisation
  ros::init(argc, argv, "drone_test_main_controller");

  ros::NodeHandle n;

  DroneController drone; //objet DroneController

  initscr(); // initialisation de la fenêtre 

  cbreak();
  noecho();
  nodelay(stdscr, TRUE);

  ros::Rate loop_rate(10);

  while(ros::ok())
  {
      if (kbhit()) { // une touche est pressée

            switch(getch()) // valeur ASCII
            {
              case 32: // espace
	      drone.sendTakeoff();
              break;

              case 104: // h
	      drone.sendLand();
              break;

              case 102: // f
	      drone.sendEmergency();
              break;

              case 119: // w
	      drone.sendPilote(0.2,0,0,0);
              break;

              case 115: // s
	      drone.sendPilote(-0.2,0,0,0);
              break;

              case 97: // a
	      drone.sendPilote(0,0.2,0,0);
              break;

              case 100: // d
	      drone.sendPilote(0,-0.2,0,0);
              break;

              case 113: // q
	      drone.sendPilote(0,0,0.2,0);
              break;

              case 101: // e
	      drone.sendPilote(0,0,-0.2,0);
              break;

              case 114: // r
	      drone.sendPilote(0,0,0,0.2);
              break;

              case 116: // t
	      drone.sendPilote(0,0,0,-0.2);
              break;

	      case 98: // b
	      drone.AutoDetect();
              break;

	      case 106: // j
	      drone.AutoDetectCouleur();
              break;

	      case 99: // c
	      drone.AutoPIDDetect();
              break;

	      

	      default:
              break;
            }
            flushinp(); // on vide le buffer de getch
     }
     else // si l'utilisateur n'appuie pas sur une touche
     {
	
     }
     ros::spinOnce();  
     loop_rate.sleep();
  }
  endwin();
}
