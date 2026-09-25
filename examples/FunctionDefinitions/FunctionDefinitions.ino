#include <Enes100.h>

/*
  ENES100 R4 FUNCTION REFERENCE

  Startup:
    bool Enes100.begin(teamName, missionType, arucoId, roomNumber);
    void Enes100.setDebug(bool enabled);

  Connection / pose:
    bool  Enes100.isConnected();
    byte  Enes100.state();
    bool  Enes100.isVisible();
    float Enes100.getX();
    float Enes100.getY();
    float Enes100.getTheta();
    bool  Enes100.updateLocation();
    Coordinate Enes100.location;

  Vision-system console output:
    Enes100.print(value1, value2, ...);
    Enes100.println(value1, value2, ...);
    Enes100.println();

  Mission output overloads:
    Enes100.mission(type, intValue);
    Enes100.mission(type, doubleValue);
    Enes100.mission(type, charValue);
    Enes100.mission(type, CoordinateValue);

  UNO R4 diagnostics:
    bool        Enes100.deviceKnown();
    const char* Enes100.hostname();
    String      Enes100.macAddress();
    const char* Enes100.serverAddress();
    uint32_t    Enes100.droppedMessages();
    bool        Enes100.debugStatus(Enes100R4DebugStatus& status);
    void        Enes100.debugDump(Stream& output);

  ML compatibility:
    int  Enes100.MLGetPrediction(int modelIndex);     // R4 client returns -1
    void Enes100.MLCaptureTrainingImage(label);
*/

const char TEAM_NAME[] = "Your Team Name";
const int ARUCO_ID = 205;
const int ROOM_NUMBER = 1116;

// This is also an example of defining your own Arduino helper function.
void printPoseToUSB() {
  if (!Enes100.isVisible()) {
    Serial.println("ArUco marker not visible");
    return;
  }

  Serial.print("x="); Serial.print(Enes100.getX());
  Serial.print(" y="); Serial.print(Enes100.getY());
  Serial.print(" theta="); Serial.println(Enes100.getTheta());
}

void setup() {
  Serial.begin(115200);
  Enes100.setDebug(false);
  Enes100.begin(TEAM_NAME, MATERIAL, ARUCO_ID, ROOM_NUMBER);

  // If using an Arduino Uno with an external WiFi module, you can use this instead
  // to explicitly assign the WiFi module TX and RX pins:
  // Enes100.begin(TEAM_NAME, MATERIAL, ARUCO_ID, ROOM_NUMBER, 3, 2);

  while (!Enes100.isConnected()) delay(10);

  // print()/println() can take several arguments. Spaces are inserted between
  // arguments automatically by this R4 library.
  Enes100.println("Connected as", Enes100.hostname(), "MAC", Enes100.macAddress());

  // Example Material mission messages:
  Enes100.mission(WEIGHT, LIGHT);
  Enes100.mission(MATERIAL_TYPE, FOAM);

  // Coordinate object example:
  Coordinate point(1.25, 0.75, 0.0);
  Serial.print("Coordinate example x = ");
  Serial.println(point.x);

  Enes100.debugDump(Serial);
}

void loop() {
  printPoseToUSB();

  Enes100R4DebugStatus status;
  if (Enes100.debugStatus(status)) {
    Serial.print("RSSI = ");
    Serial.print(status.rssi);
    Serial.print(" dBm, dropped messages = ");
    Serial.println(status.droppedMessages);
  }

  delay(1000);
}
