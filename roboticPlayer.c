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

typedef struct led {
    int ledState;
} led_struct;

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