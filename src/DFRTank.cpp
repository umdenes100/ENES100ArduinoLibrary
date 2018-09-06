#include "DFRTank.h"

DFRTank::DFRTank() {

};

void DFRTank::init(){
    pinMode(M1SpeedPin, OUTPUT);
    pinMode(M1DirectionPin, OUTPUT);
    pinMode(M2SpeedPin, OUTPUT);
    pinMode(M2DirectionPin, OUTPUT);
};

void DFRTank::setLeftMotorPWM(int pwm) {

    if (pwm > 255) {
        pwm = 255;
    } else if (pwm < -255) {
        pwm = -255;
    }
  
    analogWrite (M1SpeedPin, abs(pwm));      //PWM Speed Control

    if(pwm >= 0) {
        digitalWrite(M1DirectionPin,HIGH); 
    } 
    else {
        digitalWrite(M1DirectionPin,LOW);
    }
};

void DFRTank::setRightMotorPWM(int pwm) {

    if (pwm > 255) {
        pwm = 255;
    } else if (pwm < -255) {
        pwm = -255;
    }

    analogWrite (M2SpeedPin, abs(pwm));      //PWM Speed Control

    if(pwm >= 0) {
        digitalWrite(M2DirectionPin,HIGH); 
    } 
    else {
        digitalWrite(M2DirectionPin,LOW);
    }
};

void DFRTank::turnOffMotors(){
    digitalWrite(M1SpeedPin,LOW);
    digitalWrite(M2SpeedPin,LOW);
};
