// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

// Left Part | Forward part
int motorA1 = 13;
int motorA2 = 12;
int motorAPWM = 11;

int motorB1 = 10;
int motorB2 = 9;
int motorBPWM = 8;

// Right Part || Backward part
int motorAA1 = 7;
int motorAA2 = 6;
int motorAAPWM = 5;

int motorBB1 = 4;
int motorBB2 = 3;
int motorBBPWM = 2;

void setup() {
  Serial.begin(9600);
  Wire.begin(8);  // join i2c bus with address #8
  //Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
  Serial.print("Slave Ready");

  // Motors
  // left side
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorAPWM, OUTPUT);

  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorBPWM, OUTPUT);

  // right side
  pinMode(motorAA1, OUTPUT);
  pinMode(motorAA2, OUTPUT);
  pinMode(motorAAPWM, OUTPUT);

  pinMode(motorBB1, OUTPUT);
  pinMode(motorBB2, OUTPUT);
  pinMode(motorBBPWM, OUTPUT);

  // Left Part || Forward part
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

  // Right Part || Backward part
  digitalWrite(motorAA1, HIGH);
  digitalWrite(motorAA2, LOW);

  digitalWrite(motorBB1, HIGH);
  digitalWrite(motorBB2, LOW);
}

void loop() {
  //delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
// void requestEvent() {
//   Wire.write("hello "); // respond with message of 6 bytes
//   // as expected by master
// }

//     analogWrite(motorAPWM, 127);
//     analogWrite(motorBPWM, 127);

//     analogWrite(motorBBPWM, 127);

//     digitalWrite(motorA1, LOW);
//     digitalWrite(motorA2, HIGH);

//     digitalWrite(motorB1, LOW);
//     digitalWrite(motorB2, HIGH);

//     digitalWrite(motorBB1, LOW);
//     digitalWrite(motorBB2, HIGH);


//     analogWrite(motorAPWM, 127);
//     analogWrite(motorBPWM, 127);

//     analogWrite(motorBBPWM, 127);



void receiveEvent(size_t howMany) {
  (void)howMany;
  while (1 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  int x = Wire.read();
  Serial.println(x);

  if (x == 102) {
    Serial.println("Forward!!!");

    // left side
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);

    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);

    analogWrite(motorAPWM, 127);
    analogWrite(motorBPWM, 127);

    // right side
    digitalWrite(motorAA1, HIGH);
    digitalWrite(motorAA2, LOW);

    digitalWrite(motorBB1, LOW);
    digitalWrite(motorBB2, HIGH);

    analogWrite(motorAAPWM, 127);
    analogWrite(motorBBPWM, 127);
  }
  if (x == 98) {
    Serial.println("Backward!!!");

    
    // left side
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);

    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);

    analogWrite(motorAPWM, 127);
    analogWrite(motorBPWM, 127);

    // right side

    digitalWrite(motorAA1, LOW);
    digitalWrite(motorAA2, HIGH);

    digitalWrite(motorBB1, HIGH);
    digitalWrite(motorBB2, LOW);

    analogWrite(motorAAPWM, 127);
    analogWrite(motorBBPWM, 127);
  }
  if (x == 101) {
    // left side
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);

    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);

    analogWrite(motorAPWM, 0);
    analogWrite(motorBPWM, 0);

    // right side
    digitalWrite(motorAA1, HIGH);
    digitalWrite(motorAA2, LOW);

    digitalWrite(motorBB1, HIGH);
    digitalWrite(motorBB2, LOW);

    analogWrite(motorAAPWM, 0);
    analogWrite(motorBBPWM, 0);
  }
}