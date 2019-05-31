#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//#define BUFFER_LENGTH 256

FILE * device;
//int stringToSend[BUFFER_LENGTH];
int integerFromUser;

typedef struct led {
    int ledState;
} led_struct;

int main(int argc, char const *argv[]) {

  led_struct led_s;
  led_s.ledState = 0;

  while(1) {
      printf("Type in a short string to send to the kernel module:\n");
      scanf("%d", &integerFromUser);
      led_s.ledState = integerFromUser;
      device = fopen("/dev/arduino0","w+");
      if(device != NULL){
          fwrite((const void *) &led_s, sizeof(led_struct), 1, device);
          fclose(device);
          printf("written to device %d\n", integerFromUser);
      }
      else {
          printf("could not writte to device\n");
      }
  }

  return 0;




/*
  int ret, fd;


  printf("Starting device test code example...\n");
  fd = open("/dev/arduino1", "r+");             // Open the device with read/write access
  if (fd < 0){
     perror("Failed to open the device...");
     return errno;
  }
          // Read in a string (with spaces)
  printf("Writing message to the device [%s].\n", stringToSend);
  ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
  if (ret < 0){
     perror("Failed to write the message to the device.");
     return errno;
  }

  return 0;
  */
}
