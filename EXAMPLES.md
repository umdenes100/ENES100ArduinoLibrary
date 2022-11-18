# Initializing the Wifi Module and Updating the Location
```arduino
#include "Enes100.h"
void setup() {
    Enes100.begin("It's lit", FIRE, 3, 8, 9);  // Aruco ID of 3. Rx Pin of 8. Tx Pin of 9.
    Enes100.println("Connected!");
}
void loop(){
    while (!Enes100.updateLocation()); //will keep running until succesfully update location
    Enes100.print("X coord:");
    Enes100.println(Enes100.location.x);
    Enes100.print("Y coord:");
    Enes100.println(Enes100.location.y);
    Enes100.print("Theta coord:");
    Enes100.println(Enes100.location.theta);
    delay(1000); //wait 1 second in order to not overload the vision system
}
```
# Setting up an Ultrasoni Sensor

```arduino
#*/
// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
```

# Note: The below code is pseudocode. Its purpose is to help you understand how to do certain actions. There are multiple ways to do this.
# Drive forward until you reach a certain x and y coordinate.
```arduino
//Setting a target of x = 2.5, y = 1.5
while (!Enes100.updateLocation()); //will keep running until succesfully update location
while(Enes100.location.x < 2.5) {  //move to x = 2.5
    Enes100.updateLocation();
    moveForward();
}
stop();
if(Enes100.locaiton.y > 1.5) { //if we are above the target, move down, otherwise move up.
   Enes100.updateLocation();
   moveDown();
} else {
    Enes100.updateLocaiton();
    moveUp();
}

```
# Rotate to a certain theta coordinate
```arduino
//Setting target of theta = 1.5, but will need to set a range
while(Enes100.location.theta < 1.45 || Enes100.location.theta > 1.55) {
    //What command am I missing here???
    rotate();
}
```

