#include "Enes100.h"

static unsigned long lastPrintMs = 0;

void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("Before begin");
    Enes100.begin("Name pending...", DATA, 205, 1116, 3, 2);
    Serial.println("After begin");
}

void loop() {
    if (millis() - lastPrintMs >= 1500) {
        lastPrintMs = millis();

        Serial.print("state = ");
        Serial.println((int)Enes100.state());

        Enes100.debugDump(Serial);
    }

    delay(50);
}