#include "Enes100.h"

void setup() {
    // Initialize Enes100 library
    // Team Name, Mission Type, Marker ID, TX Pin, RX Pin
    Enes100.begin("It's Lit", FIRE, 3, 10, 11);

    Enes100.print("Destination is at (");
    Enes100.print(Enes100.destination.x);
    Enes100.print(", ");
    Enes100.print(Enes100.destination.y);
    Enes100.println(")");
    
    // Transmit the number of candles that are lit
    Enes100.mission(4);
    
    // Any other setup code...
}

void loop() {
    // Update the OSV's current location
    while(!Enes100.updateLocation()) {
        // OSV's location was not found
        Enes100.println("404 Not Found");
    }
    Enes100.print("OSV is at (");
    Enes100.print(Enes100.location.x);
    Enes100.print(", ");
    Enes100.print(Enes100.location.y);
    Enes100.print(", ");
    Enes100.print(Enes100.location.theta);
    Enes100.println(")");    
}
