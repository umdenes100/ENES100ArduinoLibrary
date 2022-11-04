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
