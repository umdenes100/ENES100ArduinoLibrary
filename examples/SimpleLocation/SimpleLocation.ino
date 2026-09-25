#include <Enes100.h>

void setup() {
  Serial.begin(115200);
  Enes100.begin("Your Team Name", MATERIAL, 205, 1116);

  // If using an Arduino Uno with an external WiFi module, you can use this instead
  // to explicitly assign the WiFi module TX and RX pins:
  // Enes100.begin("Your Team Name", MATERIAL, 205, 1116, 3, 2);
}

void loop() {
  if (Enes100.isConnected() && Enes100.isVisible()) {
    Serial.print("x = ");
    Serial.print(Enes100.getX());
    Serial.print(", y = ");
    Serial.print(Enes100.getY());
    Serial.print(", theta = ");
    Serial.println(Enes100.getTheta());
  }
  delay(100);
}
