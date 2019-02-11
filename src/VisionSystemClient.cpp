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
  mSerial->write((byte) 0);
  mSerial->write(255);
  mSerial->flush();
  
  return receive(NULL);
}

bool VisionSystemClient::begin(const char* teamName, int teamType, int markerId, int rxPin, int txPin) {
  Serial.println("begin");
  mMarkerId = markerId;
  mSerial = new SoftwareSerial(rxPin, txPin);
  mSerial->begin(9600);
  
  mSerial->write(2);
  mSerial->write(teamType);
  mSerial->write(teamName);
  mSerial->write(255);
  mSerial->flush();
  
  return receive(&destination);
}

bool VisionSystemClient::updateLocation() {
  mSerial->write(4);
  mSerial->write((char*) &mMarkerId, 2);
  mSerial->write(255);
  mSerial->flush();
  
  return receive(&location);
}

bool VisionSystemClient::mission(int message) {
  mSerial->write(6);
  mSerial->print(message);
  mSerial->write(255);
  mSerial->flush();
  
  return receive(NULL);
}

bool VisionSystemClient::mission(double message) {
  mSerial->write(6);
  mSerial->print(message);
  mSerial->write(255);
  mSerial->flush();
  
  return receive(NULL);
}

bool VisionSystemClient::mission(Coordinate& message) {
  mSerial->write(6);
  mSerial->print(message.x);
  mSerial->print(',');
  mSerial->print(message.y);
  mSerial->print(',');
  mSerial->print(message.theta);
  mSerial->write(255);
  mSerial->flush();
  
  return receive(NULL);
}

void VisionSystemClient::print(const char* message) {
  mSerial->write(8);
  mSerial->write(message);
  mSerial->write(255);
  mSerial->flush();
}

void VisionSystemClient::print(int message) {
  mSerial->write(8);
  mSerial->print(message);
  mSerial->write(255);
  mSerial->flush();
}

void VisionSystemClient::print(double message) {
  mSerial->write(8);
  mSerial->print(message);
  mSerial->write(255);
  mSerial->flush();
}

void VisionSystemClient::println(const char* message) {
  mSerial->write(8);
  mSerial->print(message);
  mSerial->write('\n');
  mSerial->write(255);
  mSerial->flush();
}

void VisionSystemClient::println(int message) {
  mSerial->write(8);
  mSerial->print(message);
  mSerial->write('\n');
  mSerial->write(255);
  mSerial->flush();
}

void VisionSystemClient::println(double message) {
  mSerial->write(8);
  mSerial->print(message);
  mSerial->write('\n');
  mSerial->write(255);
  mSerial->flush();
}

bool VisionSystemClient::receive(Coordinate* coordinate) {
  unsigned long start = millis();
  int pos = 0;
  byte buffer[13];
  
  while ((millis() - start) < 300 && pos < 13) {
    if (mSerial->available()) {
      buffer[pos++] = mSerial->read();
      if (buffer[0] == 1 || buffer[0] == 7 || buffer[0] == 9) {
        return coordinate == NULL;
      }
    }
  }
  
  coordinate->x = *(float *) (buffer + 1);
  coordinate->y = *(float *) (buffer + 5);
  coordinate->theta = *(float *) (buffer + 9);
  
  return true;
}
