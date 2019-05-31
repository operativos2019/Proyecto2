/*#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];
   printf("Starting device test code example...\n");
   fd = open("/dev/arduino1", "r+");             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   printf("Type in a short string to send to the kernel module:\n");
   scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)
   printf("Writing message to the device [%s].\n", stringToSend);
   ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }

   printf("Press ENTER to read back from the device...\n");
   getchar();

   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("The received message is: [%s]\n", receive);
   printf("End of the program\n");
   return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

FILE * device;

typedef struct led {
    uint8_t ledState;
} led_struct;

int main(int argc, char const *argv[]) {

  led_struct led_s;
  led_s.ledState = 1;

  device = fopen("/dev/arduino0","w+");
  if(device != NULL){
      fwrite(( void *) &led_s, sizeof(led_struct), 1, device);
      fclose(device);
      printf("written to device\n");
   }
   else {
       printf("could not writte to device\n");
   }

  return 0;
}



/*
#include <stdio.h>
#include <string.h>

int main () {
   FILE *fp;
   char c[] = "this is tutorialspoint";
   char buffer[100];

   // Open file for both reading and writing
   fp = fopen("/dev/arduino1", "w+");

   // Write data to the file
   fwrite(c, strlen(c) + 1, 1, fp);

   // Seek to the beginning of the file
   fseek(fp, 0, SEEK_SET);

   // Read and display data
   fread(buffer, strlen(c)+1, 1, fp);
   printf("%s\n", buffer);
   fclose(fp);

   return(0);
}*/
