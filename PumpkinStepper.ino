
/*
 Stepper Motor Control

 */

#include <AccelStepper.h>
#define HALFSTEP 8

// motor pins
#define motorPin1  D2     // IN1 on the ULN2003 driver 1
#define motorPin2  D3     // IN2 on the ULN2003 driver 1
#define motorPin3  D4     // IN3 on the ULN2003 driver 1
#define motorPin4  D5     // IN4 on the ULN2003 driver 1

#define motorPin5  D8     // IN1 on the ULN2003 driver 2
#define motorPin6  D9     // IN2 on the ULN2003 driver 2
#define motorPin7  D10    // IN3 on the ULN2003 driver 2
#define motorPin8  D11    // IN4 on the ULN2003 driver 2

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper eyesStepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper tongueStepper(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

int eyesSteps = 900; // keep track of the step count for motor 1
int eyesStepperSpeed = 1000; //speed of the stepper (steps per second)
int eyesDir = 1;
int eyesTarget = eyesSteps;

unsigned long previousEyeMovementMillis = 0;
unsigned long eyeMovementInterval = 25000;

int tongueSteps = 700; // number of steps for a 90 degree turn
int tongueStepperSpeed = 1000; //speed of the stepper (steps per second)
int tongueDir = 1;
int tongueTarget = tongueSteps;

unsigned long previousTongueInMillis = 0;
unsigned long previousTongueOutMillis = 0;
const long tongueOutInterval = 1000;
unsigned long tongueInInterval = 25000;

void setup() {
  Serial.begin(9600);

  eyesStepper.setMaxSpeed(2000.0);
  eyesStepper.move(1);  // I found this necessary
  eyesStepper.setSpeed(tongueStepperSpeed);
  eyesStepper.setAcceleration(100.0);

  tongueStepper.setMaxSpeed(2000.0);
  tongueStepper.move(-1);  // I found this necessary
  tongueStepper.setSpeed(tongueStepperSpeed);
  tongueStepper.setAcceleration(100.0);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousEyeMovementMillis >= eyeMovementInterval) {
    if(eyesSteps == 0){
      switch(eyesTarget){
        case 700:
          eyesTarget = -700;
          Serial.println("Eyes Right");
          break;
        default:
          eyesTarget = 700;
          Serial.println("Eyes Left");
          break;
      }
      Serial.print("Eyes Target ");
      Serial.println(eyesTarget);
  
      eyesStepper.move(eyesTarget);
      eyesStepper.setSpeed(eyesStepperSpeed);
  
      previousEyeMovementMillis = currentMillis;
      eyeMovementInterval = random(300, 5000);
    }
  }
  
    eyesSteps = eyesStepper.distanceToGo();
    eyesStepper.runSpeedToPosition();
    
    if (tongueSteps == 0) {
      if(tongueDir == 0){
        if (currentMillis - previousTongueInMillis >= tongueInInterval) {
          tongueDir = 1;
          tongueTarget = -900;
          
          Serial.println("Tongue Out");
          previousTongueOutMillis = currentMillis;

          Serial.print("Tongue Target ");
          Serial.println(tongueTarget);
          
          tongueStepper.move(tongueTarget);
          tongueStepper.setSpeed(tongueStepperSpeed);
        }
      }else{
        if (currentMillis - previousTongueOutMillis >= tongueOutInterval) {
          tongueDir = 0;
          tongueTarget = 900;
          Serial.println("Tongue In");
          previousTongueInMillis = currentMillis;
          tongueInInterval = random(10000, 45000);

          Serial.print("Tongue Target ");
          Serial.println(tongueTarget);
          
          tongueStepper.move(tongueTarget);
          tongueStepper.setSpeed(tongueStepperSpeed);
        }
      }
    }
    
    tongueSteps = tongueStepper.distanceToGo();
    tongueStepper.runSpeedToPosition();
}

