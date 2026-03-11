#include "Enes100.h"

static bool lastConnected = false;
static unsigned long lastStatusPrintMs = 0;

void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("Before begin");
    Enes100.begin("Name pending...", DATA, 205, 1120, 3, 2);
    Serial.println("After begin");
}

void loop() {
    const byte s = Enes100.state();
    const bool connected = (s == 0x01);

    if (connected != lastConnected) {
        lastConnected = connected;

        if (connected) {
            Serial.println("Vision system connected");
            Enes100.println("Vision system connected");
        } else {
            Serial.println("Vision system disconnected / reconnecting...");
        }
    }

    if (millis() - lastStatusPrintMs >= 1000) {
        lastStatusPrintMs = millis();

        Serial.print("state = ");
        Serial.print((int)s);
        Serial.print("  connected = ");
        Serial.println(connected ? "true" : "false");
    }

    if (!connected) {
        delay(100);
        return;
    }

    float x = Enes100.getX();
    float y = Enes100.getY();
    float t = Enes100.getTheta();
    bool v = Enes100.isVisible();

    if (v) {
        Enes100.print(x);
        Enes100.print(",");
        Enes100.print(y);
        Enes100.print(",");
        Enes100.println(t);

        Serial.print("Visible: ");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(", ");
        Serial.println(t);
    } else {
        Enes100.println("Not visible");
        Serial.println("Not visible");
    }

    delay(1000);
}