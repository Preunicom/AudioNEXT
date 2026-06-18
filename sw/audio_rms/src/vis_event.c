//##INSERT YOUR CODE HERE 

/****************** Include Files ********************/
#include <stdint.h>
//begin edit Maximilian Hafeneder
//#include "vis_event.h" //do not include the own header here since this confilicats with the gloabal definition of the event/sync var


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/
typedef uint8_t vis_event_type;


/************************** Variable Definitions ***************************/
volatile vis_event_type vis_event=0; //definition of global var 


/************************** Function Definitions ***************************/
void vis_setEvent(vis_event_type ev){ //hand over 1
  vis_event=ev; //just a single instruction (atomic) -> no critical section required so far 
}

void vis_clearEvent(vis_event_type ev){ //hand over 0
  vis_event=ev; //just a single instruction (atomic) -> no critical section required so far 
}

uint8_t vis_eventIsSet(){
	return vis_event;
}
//end edit Maximilian Hafeneder

//##INSERT YOUR CODE HERE END