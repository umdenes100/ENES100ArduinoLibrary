#if !defined(ARDUINO_UNOR4_WIFI)
#error "Machine_Learning_R4 only works on the Arduino UNO R4 WiFi. On other boards use the Machine_Learning example."
#endif

#include "Enes100.h"

/*
 * Machine Learning on the Arduino UNO R4 WiFi with the ENES100 ML camera.
 *
 * This sketch is for the UNO R4 WiFi only. On a classic Uno, use the
 * Machine_Learning example instead.
 *
 * In this mode the ML camera is the WiFi module, exactly like on a classic Uno:
 * every Enes100 call (location, print, mission, ML) goes through the camera,
 * and the R4's onboard WiFi is not used.
 *
 * Wiring (camera board header -> UNO R4 WiFi):
 *   GD (GND) -> GND
 *   5V       -> 5V
 *   TX       -> D0 (RX)
 *   RX       -> D1 (TX)
 *
 * Put the switch on the camera board in the vision position (not training).
 * D0/D1 are Serial1 on the R4, so do not use them for anything else. Unlike a
 * classic Uno, you can upload and use the Serial Monitor with the camera
 * connected.
 */

void setup() {
    // Initialize Enes100 Library:

    // Team Name, Mission Type, Marker ID, Room Number, Serial port the camera is wired to
    Enes100.begin("asimple2dmodel", SEED, 381, 1116, Serial1);

    /*
     * For ML, make sure you upload your model to the ENES100 website before running
     * your Arduino script. Additionally, make sure your team name matches EXACTLY to the
     * team name you submitted on the website
     */

    // Wait for the camera to receive our team information.
    while (!Enes100.isConnected()) {
        delay(100);
    }

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
