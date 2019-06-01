#include <Servo.h>

#define LINE_BUFFER_LENGTH 512

int pos = 0;

Servo servo_8;
Servo servo_9;
Servo servo_10;
int electroIman_Pin = 11;

/* Structures, global variables 
typedef struct led {
  uint8_t ledState;
} point_struct;*/

//struct used to comunicate
typedef struct message {
    int servo1; //arriba y abajo 320-350
    int servo2; //derecha e izquierda 190-10
    int servo3; //adelante y atras 0-180
    int magnet; //iman 1 o 0
} message_struct;

/* actual point */
mesage_struct* lastMessage;
mesage_struct* currentMessage; 

int struct_size = (int) sizeof(struct message);

void movDown(int coor){
  //coor = 10 full
  for (pos = 120; pos >= coor; pos -= 1) {
    // tell servo to go to position in variable 'pos'
    servo_8.write(120);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}

void movUp(int coor){
  //coor = 120 full
  for (pos = 90; pos <= coor; pos += 1) {
    // tell servo to go to position in variable 'pos'
    servo_8.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}



void movStop(){
  //reposo para servo 9
  servo_9.write(90);
  delay(1000);
  //reposo para servo 10
  servo_10.write(100);
  delay(1000);
  //reposo para servo 8
  servo_8.write(10);
  delay(1000);
}


void movRight(int coor){
  //coor = 10 full
  for (pos = 100; pos >= coor; pos -= 1) {
    // tell servo to go to position in variable 'pos'
    servo_10.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}

void movLeft(int coor){
  //coor = 190 full
  for (pos = 100; pos <= coor; pos += 1) {
    // tell servo to go to position in variable 'pos'
    servo_10.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}

void movForward(int coor){
  //coor = 350 full
  for (pos = 320; pos <= coor; pos += 1) {
    // tell servo to go to position in variable 'pos'
    servo_9.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
  
}

void movBackward(int coor){
  //coor = 90 full
  for (pos = 320; pos >= coor; pos -= 1) {
    // tell servo to go to position in variable 'pos'
    servo_9.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
  //movRight(coor);
}

void magOn(){
 digitalWrite(electroIman_Pin, HIGH);
 delay(1000);
}

void magOff(){
 digitalWrite(electroIman_Pin, LOW);
 delay(1000); 
}


/**********************
 * void setup() - Initialisations
 ***********************/
void setup() {
  lastMessage->servo1 = 90;
  lastMessage->servo2 = 100;
  lastMessage->servo3 = 10;
  servo_8.attach(8);
  servo_9.attach(9);
  servo_10.attach(10);
  pinMode(electroIman_Pin, OUTPUT);
  digitalWrite(electroIman_Pin, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

/******************************************************
Serial read and moving determination
******************************************************/

void loop(){

  if(Serial.available()){
    digitalWrite(LED_BUILTIN, 1);
    char serial_buffer [struct_size];
    Serial.readBytes(serial_buffer, struct_size);
    currentMessage = (message_struct*) &serial_buffer;
    
    if(lastMessage->servo1 > currentMessage->servo1) {
      movUp(lastMessage->servo1 - currentMessage->servo1);    
    }
    else {
      movDown(currentMessage->servo1 - lastMessage->servo1);
    }

    if(lastMessage->servo2 > currentMessage->servo2) {
      movRight(lastMessage->servo2 - currentMessage->servo2);    
    }
    else {
      movLeft(currentMessage->servo2 - lastMessage->servo2);
    }

    if(lastMessage->servo3 > currentMessage->servo3) {
      movForward(lastMessage->servo3 - currentMessage->servo3);    
    }
    else {
      movBackwar(currentMessage->servo3 - lastMessage->servo3);
    }

    if(currentMessage->magnet) {
       magOn();
    }
    else {
      magOff();
    }
    delay(500);
    digitalWrite(LED_BUILTIN, 0);
  }
}
