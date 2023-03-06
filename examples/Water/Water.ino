#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
    Enes100.begin("Water We Doing", WATER, 3, 2, 3);

    Enes100.print("Destination is at (");
    Enes100.print(Enes100.missionSite.x);
    Enes100.print(", ");
    Enes100.print(Enes100.missionSite.y);
    Enes100.println(")");
    
    // Transmit the state of the pool
    Enes100.mission(WATER_TYPE, FRESH_POLLUTED);
    // Transmit the depth of the pool in mm (20, 30, or 40)
    Enes100.mission(DEPTH, 30);
    // Any other setup code...
}

void loop() {
    float x, y, t; bool v; // Declare variables to hold the data
    x = Enes100.getX();
    y = Enes100.getY();
    t = Enes100.getTheta();
    v = Enes100.getVisibility();
    if (v) // If the ArUco marker is visible
    {
        Enes100.print(x); // print out the location
        Enes100.print(",");
        Enes100.println(y);
        Enes100.print(",");
        Enes100.println(t);
    }
    else { // otherwise
        Enes100.println("Not visible"); // print not visible
    }
    delay(1000);
}
