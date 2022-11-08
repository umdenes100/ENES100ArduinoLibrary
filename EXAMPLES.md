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
# Note: The below code is pseudocode. Its purpose is to help you understand how to do certain actions. There are multiple ways to do this.
# Drive forward until you reach a certain x and y coordinate.
```arduino
//SEtting a target of x = 2.5, y = 1.5
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