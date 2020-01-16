#include "VisionSystemClient.hpp"

Coordinate::Coordinate() {
  init(0, 0, 0);
}

Coordinate::Coordinate(double x, double y) {
  init(x, y, 0);
}

Coordinate::Coordinate(double x, double y, double theta) {
  init(x, y, theta);
}

void Coordinate::init(double x, double y, double theta) {
  this->x = x;
  this->y = y;
  this->theta = theta;
}

bool VisionSystemClient::ping() {
  mSerial->write((byte) OP_PING);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
  
  return receive(NULL);
}

bool VisionSystemClient::begin(const char* teamName, int teamType, int markerId, int rxPin, int txPin) {
  delay(2000);
  mMarkerId = markerId;
  mSerial = new SoftwareSerial(rxPin, txPin);
  mSerial->begin(9600);
  
  while (mSerial->available()) {
    mSerial->read();
  }
  
  mSerial->write(OP_BEGIN);
  mSerial->write(teamType);
  mSerial->write(teamName);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
  
  return receive(&destination);
}

bool VisionSystemClient::updateLocation() {
  mSerial->write(OP_LOCATION);
  mSerial->write((byte*) &mMarkerId, 2);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
  
  return receive(&location);
}

bool VisionSystemClient::mission(int message) {
  mSerial->write(OP_MISSION);
  mSerial->print(message);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
  
  return receive(NULL);
}

bool VisionSystemClient::mission(double message) {
  mSerial->write(OP_MISSION);
  mSerial->print(message);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
  
  return receive(NULL);
}
bool VisionSystemClient::mission(char message) {
  mSerial->write(OP_MISSION);
  mSerial->print(message);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();

  return receive(NULL);
}

bool VisionSystemClient::mission(Coordinate& message) {
  mSerial->write(OP_MISSION);
  mSerial->print(message.x);
  mSerial->print(',');
  mSerial->print(message.y);
  mSerial->print(',');
  mSerial->print(message.theta);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
  
  return receive(NULL);
}

bool VisionSystemClient::receive(Coordinate* coordinate) {
  unsigned long start = millis();
  int pos = 0;
  byte buffer[13];
  
  while (millis() - start < 120 && pos < 13) {
    if (mSerial->available()) {
      buffer[pos++] = mSerial->read();
      if (buffer[0] == 1 || buffer[0] == 7 || buffer[0] == 9) {
        return coordinate == NULL;
      } else if (pos == 13) {
        if (coordinate != NULL) {
          coordinate->x = *(float *) (buffer + 1);
          coordinate->y = *(float *) (buffer + 5);
          coordinate->theta = *(float *) (buffer + 9);
          return true;
        } else {
          return false;
        }
      }
    }
  }
  
  return false;
}
