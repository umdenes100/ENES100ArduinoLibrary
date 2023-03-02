#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library 
    // Team Name, Mission Type, Marker ID, TX Pin, RX Pin
    Enes100.begin("DATA TEAM", DATA, 3, 10, 11);

    Enes100.print("Destination is at (");
    Enes100.print(Enes100.missionSite.x);
    Enes100.print(", ");
    Enes100.print(Enes100.missionSite.y);
    Enes100.println(")");
    
    // Transmit the duty cycle of the data mission (7 for 70%)
    Enes100.mission(CYCLE, 7);
    // Transmit the magnetism of the data mission (in this case, not magnetic)
    Enes100.mission(MAGNETISM, NOT_MAGNETIC);
    // Any other setup code...
}

void loop() {
    // Update the OSV's current location
    while (!Enes100.updateLocation()) {
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
