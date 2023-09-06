#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
    Enes100.begin("Team WALL-E", MACHINE_LEARNING, 114, 10, 11);

    Enes100.print("Destination is at (");
    Enes100.print(Enes100.missionSite.x);
    Enes100.print(", ");
    Enes100.print(Enes100.missionSite.y);
    Enes100.println(")");
}
bool sent = false;
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

    if(millis() > 5000 && !sent) { // After 5 seconds, capture an image.
        Enes100.MLCaptureTrainingImage("dirt");
        //int result = Enes100.MLGetPrediction(); // How you get the prediction from your model given the camera image.
        sent = true; //We only will request to send one image.
        Enes100.mission(PERCENTAGE, 50); //This how you should sent the percentage of the mission site contains plantable substrate.
        Enes100.mission(LOCATION, Coordinate(2000, 1900)); //This is how you should send the location of each mission site that contains a plantable substrate.
        Enes100.mission(LOCATION, Coordinate(2000, 2100)); //Run this command multiple times for each plantable substrate location.
    }
}
