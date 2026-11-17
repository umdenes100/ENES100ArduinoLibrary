#pragma once

// UNO R4 WiFi "camera as WiFi module" link.
//
// Speaks the classic ENES100 WiFi-module serial protocol (the one used by
// VisionSystemClient on the Uno/Mega/Nano) over a hardware UART, so an UNO R4
// WiFi can use the ENES100 ML camera exactly like a classic Uno does. The
// camera joins WiFi and talks to the vision system; the R4's onboard WiFi is
// not used in this mode.
//
// Students do not use this class directly. It is selected with:
//   Enes100.begin("Team Name", SEED, 381, 1116, Serial1);

#if defined(ARDUINO_UNOR4_WIFI)

#include <Arduino.h>

class Coordinate;

class Enes100R4CameraLink {
public:
  // `location` is the Enes100.location object to keep up to date.
  void begin(const char* teamName, byte teamType, int markerId, int roomNumber,
             HardwareSerial& port, Coordinate* location);

  bool isConnected();
  byte state();

  float getX();
  float getY();
  float getTheta();
  bool isVisible();
  bool updateLocation();

  void mission(int type, int message);
  void mission(int type, double message);
  void mission(int type, char message);
  void mission(int type, double x, double y, double theta);

  int MLGetPrediction(int modelIndex);

  void print(const char* message, bool newline);

  void debugDump(Stream& out);

private:
  void updateIfNeeded();
  void readBytes(byte* buffer, int length);
  void clearInput(uint16_t quietMs = 4, uint16_t maxMs = 40);
  byte queryState(uint16_t timeoutMs = 30);
  void sendBeginPacket();
  void maintainConnection(bool aggressive = false);

  bool visible = false;
  float mX = -1;
  float mY = -1;
  float mTheta = -1;
  Coordinate* mLocation = nullptr;

  uint32_t lastUpdate = 0;
  uint32_t mLastBeginSendMs = 0;
  uint32_t mLastStatePollMs = 0;
  byte mLastState = 0xFF;

  int mMarkerId = 0;
  int mRoomNumber = 0;
  byte mTeamType = 0;
  char mTeamName[64] = {0};
  bool mConfigValid = false;

  HardwareSerial* mSerial = nullptr;
};

#endif // ARDUINO_UNOR4_WIFI
