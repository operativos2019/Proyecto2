/** 
 * This code is the implementation of the device library that takes care of providing abstractions 
 * to talk with the robotic player through the device driver
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

FILE * device;
int integerFromUser;
/*
typedef struct messageContainer {
    int message;
} messageContainer_struct;
*/


// Struct that will contained the message sent 
typedef struct led {
    int ledState;
} led_struct;


//Test method that writes a 1 into the driver
void sendOne() {
  led_struct led_s;
  led_s.ledState = 1;

  device = fopen("/dev/arduino1","w+");
  if(device != NULL){
      fwrite(( void *) &led_s, sizeof(led_struct), 1, device);
      fclose(device);
      printf("written to device\n");
   }
   else {
       printf("could not writte to device\n");
   }
}

//Picking up a piece on the current position
void pick(){
    printf("Picking");
    return;
}

//Dropping a piece on the current location
void drop(){
    printf("Dropping");
    return;
}

//Moving the robotic player to a coordinates
void move(int x, int y){
    printf("Moving to %d %d", y, x);
    return;
}