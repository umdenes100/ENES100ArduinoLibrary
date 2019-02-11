#ifndef VisionSystemClient_hpp
#define VisionSystemClient_hpp

#include "Arduino.h"
#include "SoftwareSerial.h"

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
  bool mission(Coordinate& message);
  void print(const char* message);
  void print(int message);
  void print(double message);
  void println(const char* message);
  void println(int message);
  void println(double message);
  
  Coordinate location;
  Coordinate destination;
  
private:
  bool receive(Coordinate* coordinate = NULL);
  
  int mMarkerId;
  SoftwareSerial* mSerial;
};

#endif /* VisionSystemClient_hpp */
