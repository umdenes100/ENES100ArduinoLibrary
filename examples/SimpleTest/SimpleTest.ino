#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
    Enes100.begin("Simple Test", DATA, 7, 10, 11);

    Enes100.print(F("Destination is at ("));
    Enes100.print(Enes100.missionSite.x);
    Enes100.print(", ");
    Enes100.print(Enes100.missionSite.y);
    Enes100.println(")");
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
        Enes100.println(F("Not visible")); // print not visible
    }
    delay(1000);
}