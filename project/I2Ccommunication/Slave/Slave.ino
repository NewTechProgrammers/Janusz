#include <Wire.h>

class Controller {
  public:
    int C_A1 = -1;  int C_B1 = -1;
    int C_A2 = -1;  int C_B2 = -1;
    int C_APWM = -1;  int C_BPWM = -1;
    bool APoles = true;  bool BPoles = true;

    Controller(int _C_A1, int _C_A2, int _C_APWM, int _C_B1, int _C_B2, int _C_BPWM, bool _APoles=true, bool _BPoles = true) : C_A1(_C_A1), C_A2(_C_A2), C_APWM(_C_APWM), APoles(_APoles), C_B1(_C_B1), C_B2(_C_B2), C_BPWM(_C_BPWM), BPoles(_BPoles) {}
    Controller(int _C_A1, int _C_A2, int _C_APWM, bool _APoles = true) : C_A1(_C_A1), C_A2(_C_A2), C_APWM(_C_APWM), APoles(_APoles) {}
    
    void runForwardAandB(int PWMSpeed = 50, int APWMSpeed = -1, int BPWMSpeed = -1) {
        if (APWMSpeed == -1) { APWMSpeed = PWMSpeed; } if (BPWMSpeed == -1) { BPWMSpeed = PWMSpeed; }
        runForwardA(APWMSpeed); runForwardB(BPWMSpeed);
    }

    void runBackwardAandB(int PWMSpeed = 50, int APWMSpeed = -1, int BPWMSpeed = -1) {
        if (APWMSpeed == -1) { APWMSpeed = PWMSpeed; } if (BPWMSpeed == -1) { BPWMSpeed = PWMSpeed; }
        runBackwardA(APWMSpeed); runBackwardB(BPWMSpeed);
    }

    void stopAandB(){ stopA(); stopB(); }
    
    void runForwardA(int PWMSpeed = 50) { digitalWrite(C_A1, APoles ? HIGH : LOW); digitalWrite(C_A2, APoles ? LOW : HIGH); analogWrite(C_APWM, PWMSpeed); }

    void runBackwardA(int PWMSpeed = 50){ digitalWrite(C_A1, APoles ? LOW : HIGH); digitalWrite(C_A2, APoles ? HIGH : LOW); analogWrite(C_APWM, PWMSpeed); }

    void stopA() { digitalWrite(C_A1, APoles ? LOW : HIGH); digitalWrite(C_A2, APoles ? HIGH : LOW); analogWrite(C_APWM, 0); }
    
    void runForwardB(int PWMSpeed = 50) { digitalWrite(C_B1, BPoles ? HIGH : LOW); digitalWrite(C_B2, BPoles ? LOW : HIGH); analogWrite(C_BPWM, PWMSpeed); }

    void runBackwardB(int PWMSpeed = 50) { digitalWrite(C_B1, BPoles ? LOW : HIGH); digitalWrite(C_B2, BPoles ? HIGH : LOW); analogWrite(C_BPWM, PWMSpeed); }
    
    void stopB() { digitalWrite(C_B1, BPoles ? LOW : HIGH); digitalWrite(C_B2, BPoles ? HIGH : LOW); analogWrite(C_BPWM, 0); }
};

// Front part
int motorA1 = 13;
int motorA2 = 12;
int motorAPWM = 11;
 
int motorB1 = 10;
int motorB2 = 9;
int motorBPWM = 8;

Controller front(13, 12, 11, 10, 9, 8, false, false);

// Back part
int motorAA1 = 7;
int motorAA2 = 6;
int motorAAPWM = 5;
 
int motorBB1 = 4;
int motorBB2 = 3;
int motorBBPWM = 2;

Controller back(7, 6, 5, 4, 3, 2, true, false);
 
// Header part 
int headerA1 = 47;
int headerA2 = 43;
int headerPWM = 45;

Controller header(47, 43, 45, true);
 
void setup() {
  Serial.begin(9600);
  Wire.begin(8);  // join i2c bus with address #8
  
  Wire.onReceive(receiveEvent);
  Serial.print("Slave Ready");
 
  // Front part
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorAPWM, OUTPUT);
 
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorBPWM, OUTPUT);
 
  // Back part
  pinMode(motorAA1, OUTPUT);
  pinMode(motorAA2, OUTPUT);
  pinMode(motorAAPWM, OUTPUT);
 
  pinMode(motorBB1, OUTPUT);
  pinMode(motorBB2, OUTPUT);
  pinMode(motorBBPWM, OUTPUT);
 
  // Header
  pinMode(headerA1, OUTPUT);
  pinMode(headerA2, OUTPUT);
  pinMode(headerPWM, OUTPUT);
}
 
void loop() {}
 
void receiveEvent(size_t howMany) {
  (void)howMany;
  while (1 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  int x = Wire.read();
  Serial.println(x);
  // 'f' (102) - forward | 'l' (108) - left | 'r' (114) - right | 'b' (98) - backward | 'e' (101) - stop | 'h' (104) - header
  if (x == 102) {
    Serial.println("Forward!!!");
 
    front.runForwardAandB(255);
    back.runForwardAandB(255);
  }
 
  if (x == 108) {
    Serial.println("Left!!!");
 
    front.runBackwardA(255);
    front.runForwardB(255);
 
    back.runBackwardA(255);
    back.runForwardB(255);
  }

  if (x == 114) {
    Serial.println("Right!!!");
 
    front.runBackwardB(255);
    front.runForwardA(255);
 
    back.runBackwardB(255);
    back.runForwardA(255);
  }

  if (x == 98) {
    Serial.println("Backward!!!");

    front.runBackwardAandB(255);
    back.runBackwardAandB(255);
  }

  if (x == 101) {
    Serial.println("Stop!!!");
    digitalWrite(headerA1, HIGH);
    digitalWrite(headerA2, LOW);
 
    analogWrite(headerPWM, 0);

    front.stopAandB();
    back.stopAandB();
  }
}