#include <Enes100.h>

static unsigned long lastPrintMs = 0;
static unsigned long debugStartMs = 0;

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println("Starting ENES100...");

  Enes100.begin("Name pending...", DATA, 205, 1116);

  // If using an Arduino Uno with an external WiFi module, you can use this instead
  // to explicitly assign the WiFi module TX and RX pins:
  // Enes100.begin("Name pending...", DATA, 205, 1116, 3, 2);

  debugStartMs = millis();
  Serial.println("ENES100 begin complete. Waiting for connection...");
}

void loop() {
  if (millis() - lastPrintMs >= 1500) {
    lastPrintMs = millis();

    byte currentState = Enes100.state();
    Serial.print("state = ");
    Serial.print((int)currentState);

    if (currentState == 1) {
      Serial.println(" (connected)");
    } else if (currentState == 0) {
      Serial.println(" (module responding; connection in progress)");
    } else if (currentState == 255 && millis() - debugStartMs < 5000) {
      Serial.println(" (waiting for WiFi module to become ready)");
    } else {
      Serial.println(" (no valid module state received)");
    }

    // During the first few seconds the WiFi module may still be booting or
    // joining WiFi. A failed debug read in that window is normally transient.
    Enes100.debugDump(Serial);
  }

  delay(50);
}
