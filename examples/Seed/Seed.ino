#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
    Enes100.begin("Dirt Alert", SEED, 205, 3, 2);
    // At this point we know we are connected.
    Enes100.println("Connected...");
}
bool sent = false;
void loop() {
    float x, y, t; bool v; // Declare variables to hold the data
    //Enes100.getX will make sure you get the latest data available to you about your OTV's location.
    //The first time getX is called, X, Y, theta and visibility are queried and cached.
    //Subsequent calls return from the cache, so there is no performance gain to saving the function response to a variable.

    x = Enes100.getX();  // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
    y = Enes100.getY();  // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
    t = Enes100.getTheta();  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
    v = Enes100.isVisible(); // Is your aruco visible? True or False.

    if (v) // If the ArUco marker is visible
    {
        Enes100.print(x); // print out the location
        Enes100.print(",");
        Enes100.print(y);
        Enes100.print(",");
        Enes100.println(t);
    }
    else { // otherwise
        Enes100.println("Not visible"); // print not visible
    }

    if(millis() > 5000 && !sent) {
        int result = Enes100.MLGetPrediction(); // How you get the prediction from your model given the camera image.
        sent = true; //We only will request to send one image.
        Enes100.mission(PERCENTAGE, 50); //This how you should sent the percentage of the mission site contains plantable substrate.
        Enes100.mission(LOCATION, Coordinate(2000, 1900)); //This is how you should send the location of each mission site that contains a plantable substrate.
        Enes100.mission(LOCATION, Coordinate(2000, 2100)); //Run this command multiple times for each plantable substrate location.
    }

    delay(1000);
}