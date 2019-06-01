#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

FILE * device;
int integerFromUser;

typedef struct message {
    int servo1; //arriba y abajo 320-350
    int servo2; //derecha e izquierda 190-10
    int servo3; //adelante y atras 0-180
    int magnet; //iman 1 o 0
} message_struct;
 
void sendOne() {
  message_struct message_s;
  message_s.servo1 = 340;
  message_s.servo2 = 100;
  message_s.servo3 = 100;
  message_s.magnet = 1;

  device = fopen("/dev/arduino1","w+");
  if(device != NULL){
      fwrite(( void *) &message_s, sizeof(message_struct), 1, device);
      fclose(device);
      printf("written to device\n");
   }
   else {
       printf("could not writte to device\n");
   }
}

void sendMessage(int servo1, int servo2, int servo3, int magnet) {
  message_struct message_s;
  message_s.servo1 = servo1;
  message_s.servo2 = servo2;
  message_s.servo3 = servo3;
  message_s.magnet = magnet;

  device = fopen("/dev/arduino1","w+");
  if(device != NULL){
      fwrite(( void *) &message_s, sizeof(message_struct), 1, device);
      fclose(device);
      printf("written to device\n");
   }
   else {
       printf("could not writte to device\n");
   }
}