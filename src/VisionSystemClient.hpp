#ifndef VisionSystemClient_hpp
#define VisionSystemClient_hpp

#include "Arduino.h"
#include "SoftwareSerial.h"

// Transmission opcodes
#define OP_PING     0
#define OP_BEGIN    3
#define OP_LOCATION 5
#define OP_MISSION  6
#define OP_ML_PREDICTION 7
#define OP_PRINT    8
#define OP_ML_CAPTURE 9
//OP_CHECK is the same as OP_LOCATION but can return different first values if the value is not available or hasn't changed.
#define OP_CHECK    51


const byte FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC};

class Coordinate {
public:
  Coordinate();
  Coordinate(double x, double y);
  Coordinate(double x, double y, double theta);
  
  double x;
  double y;
  double theta;

private:
  void init(double x, double y, double theta);
};

class VisionSystemClient {
public:
  bool ping();
  bool begin(const char* teamName, byte teamType, int markerId, int wifiModuleRX, int wifiModuleTX);
  bool updateLocation();
  void mission(int type, int message);
  void mission(int type, double message);
  void mission(int type, char message);
  void mission(int type, Coordinate& message);
  int MLGetPrediction();
  void MLCaptureTrainingImage(const char * label);

  float getX();
  float getY();
  float getTheta();
  bool getVisibility();

  template <typename T>
  void print(T message);
  template <typename T>
  void println(T message);
  
  Coordinate location;
  Coordinate missionSite;
  
private:
  bool receive(Coordinate* coordinate = NULL);
  void updateIfNeeded();
  bool visible;
  void readBytes(byte* buffer, int length);
  void waitForConnection();
  uint32_t lastUpdate;
  
  int mMarkerId;
  SoftwareSerial* mSerial;
};

/**
 * Templated functions have to be in the header file
 **/
template <typename T>
void VisionSystemClient::print(T message) {
  mSerial->write(OP_PRINT);
  mSerial->print(message);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

template <typename T>
void VisionSystemClient::println(T message) {
  mSerial->write(OP_PRINT);
  mSerial->print(message);
  mSerial->write('\n');
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

#endif /* VisionSystemClient_hpp */
