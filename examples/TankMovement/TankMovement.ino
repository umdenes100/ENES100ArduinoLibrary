#include <DFRTank.h>

DFRTank tank;

void setup(){
  Serial.begin(9600);
  tank.init();
}
void loop(){

  //Move forward
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
  delay(500);

  //Turn left
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
  delay(500);

  //Stop
  tank.turnOffMotors();
  delay(500);
  
}
