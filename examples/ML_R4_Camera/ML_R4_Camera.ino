#include "Enes100.h"

#if !defined(ARDUINO_UNOR4_WIFI)
#error "This example is for the Arduino UNO R4 WiFi. For Uno/Mega with the ML camera as WiFi module, use the Machine_Learning example."
#endif

/*
 * Machine learning on the Arduino UNO R4 WiFi with the ENES100 ML camera.
 *
 * The camera takes the picture and sends it to the R4 over Serial1; the R4
 * forwards it to the vision system over its own WiFi and gets the prediction.
 *
 * Wiring (camera switch in the vision/up position, not training):
 *   ML camera TX  ->  R4 pin 0 (RX)
 *   ML camera RX  <-  R4 pin 1 (TX)
 *   ML camera GND ->  R4 GND
 * Pins 0 and 1 are reserved for the camera; use Serial (USB) for debugging.
 *
 * Before running: upload your model on the ENES100 website using EXACTLY the
 * same team name as in Enes100.begin() below, and note its model index.
 */

void setup() {
    Serial.begin(9600);

    // Team Name, Mission Type, Marker ID, Room Number
    Enes100.begin("ColorTest", MATERIAL, 205, 1201);

    Enes100.println("Connected...");
}

void loop() {
    unsigned long t0 = millis();

    //                                   Model Index
    //                                        |
    //                                        v
    int result = Enes100.MLGetPrediction(1);

    if (result >= 0) {
        // result is the category number, in the order your categories were
        // numbered when you trained the model (0, 1, 2, ...).
        Enes100.println("ML predicts category " + String(result) +
                        " (" + String(millis() - t0) + " ms)");
    } else {
        // See Enes100R4.h for the full list of ML_ERR_* codes.
        switch (Enes100.MLLastError()) {
            case ML_ERR_NOT_CONNECTED:    Serial.println("Not connected to the vision system yet"); break;
            case ML_ERR_CAMERA_NOT_FOUND: Serial.println("No camera on pins 0/1 - check wiring, power and the camera switch"); break;
            case ML_ERR_CAPTURE_FAILED:   Serial.println("Camera could not take a picture"); break;
            case ML_ERR_TRANSFER_FAILED:  Serial.println("Image was corrupted on the wire - check the camera wires"); break;
            case ML_ERR_TIMEOUT:          Enes100.println("No prediction came back - check team name, model index and upload"); break;
        }
    }

    delay(2000);
}
