#include <Servo.h>

#define LINE_BUFFER_LENGTH 512

int pos = 0;

Servo servo_8;
Servo servo_9;
Servo servo_10;
int electroIman_Pin = 11;

/* Structures, global variables */
typedef struct led {
  uint8_t ledState;
} point_struct;

/* actual point */
point_struct *actual_pos;

int struct_size = (int) sizeof(struct led);

uint8_t initialLedState = 0;

void movDown(){
  for (pos = 120; pos >= 10; pos -= 1) {
    // tell servo to go to position in variable 'pos'
    servo_8.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }    
}

void movUp(){
  for (pos = 90; pos <= 120; pos += 1) {
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
  
  //movDown();
  //movBackwards();
}

void movRight(){
  for (pos = 100; pos >= 10; pos -= 1) {
    // tell servo to go to position in variable 'pos'
    servo_10.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}

void movLeft(){
  for (pos = 100; pos <= 180; pos += 1) {
    // tell servo to go to position in variable 'pos'
    servo_10.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}

void movForward(){
  movUp();
  for (pos = 320; pos <= 350; pos += 1) {
    // tell servo to go to position in variable 'pos'
    servo_9.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}

void movBackwards(){
  movDown();
  for (pos = 320; pos >= 90; pos -= 1) {
    // tell servo to go to position in variable 'pos'
    servo_9.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
  movRight();
}

void movClose(){
  movUp();
  delay(15);
  movDown();
  delay(15);
  movForward();
  delay(15);
  movBackwards();
  delay(15);
}

void movLarge(){
  movForward();
  delay(15);
  movDown();
  delay(30);
  movUp();
  delay(15);
}


/**********************
 * void setup() - Initialisations
 ***********************/
void setup() {
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
int control;

void loop(){

  if(Serial.available()){
    digitalWrite(LED_BUILTIN, 1);
    char serial_buffer [struct_size];
    Serial.readBytes(serial_buffer, struct_size);
    actual_pos = (point_struct*) &serial_buffer;
    control = actual_pos->ledState;
    if(control == 1) {
      movDown();
    }
    else if(control == 2) {
      movUp();
    }
    else if(control == 3) {
      movStop();
    }
    else if (control == 4){
      movRight();
    }
    else if(control == 5) {
      movLeft();    
    }
    else if(control == 6) {
      movForward();
    }  
    else if(control == 7) {
      movBackwards();
    }
    else if(control == 8) {
      movClose();
    }
    else if(control == 9) {
      movLarge();
    }
    delay(1000);
    digitalWrite(LED_BUILTIN, 0);
  }
}
