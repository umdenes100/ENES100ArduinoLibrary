#ifndef VisionSystemClient_hpp
#define VisionSystemClient_hpp

#include "Arduino.h"
#include "SoftwareSerial.h"

// Transmission opcodes
#define OP_PING     0
#define OP_BEGIN    2
#define OP_LOCATION 4
#define OP_MISSION  6
#define OP_PRINT    8

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
  bool begin(const char* teamName, int teamType, int markerId, int rxPin, int txPin);
  bool updateLocation();
  bool mission(int message);
  bool mission(double message);
  bool mission(char message);
  bool mission(Coordinate& message);
  template <typename T>
  void print(T message);
  template <typename T>
  void println(T message);
  
  Coordinate location;
  Coordinate destination;
  
private:
  bool receive(Coordinate* coordinate = NULL);
  
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
