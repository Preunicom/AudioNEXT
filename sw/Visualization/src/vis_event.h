//##INSERT YOUR CODE HERE 
//begin edit Maximilian Hafeneder
#ifndef VIS_EVENT_H
#define VIS_EVENT_H

/****************** Include Files ********************/
#include <stdint.h>


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/
typedef uint8_t vis_event_type;
//volatile vis_event_type vis_event=0; //move this to global section of a c-file 
extern vis_event_type vis_event;

/************************** Function Prototypes ****************************/

void vis_setEvent(vis_event_type ev); 

void vis_clearEvent(vis_event_type ev);

uint8_t vis_eventIsSet();

/************************** Function Definitions ***************************/
       
                                         
#endif /* VIS_EVENT_H */
//end edit Maximilian Hafeneder

//##INSERT YOUR CODE HERE