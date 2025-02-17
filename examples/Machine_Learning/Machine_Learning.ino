#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library:

    // Team Name, Mission Type, Marker ID, Room Number, Wifi Module TX Pin, Wifi Module RX Pin
    Enes100.begin("asimple2dmodel", SEED, 381, 1116, 3, 2);

    /*
     * For ML, make sure you upload your model to the ENES100 website before running
     * your Arduino script. Additionally, make sure your team name matches EXACTLY to the
     * team name you submitted on the website
     */

    // At this point we know we are connected.
    Enes100.println("Connected...");

}

void loop() {

    Enes100.println("Requesting prediction...");

    unsigned long t0 = millis();

    // How you get the prediction from your model given the camera image.
    //                              Model Index
    //                                   |
    //                                   v
    int result = Enes100.MLGetPrediction(1);

    unsigned long delta_t = millis() - t0;

    Enes100.println("Results are in! ML predicts: " + String(result));
    Enes100.println("The prediction took: " + String(delta_t) + " ms");

    // Wait 5 seconds...
    delay(5000);

}
