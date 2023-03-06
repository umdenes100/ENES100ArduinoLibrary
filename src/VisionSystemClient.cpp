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
#pragma message "VisionSystemClient - Note: Using new version! Downloaded after (6/6/2023)"


bool VisionSystemClient::begin(const char* teamName, byte teamType, int markerId, int wifiModuleRX, int wifiModuleTX) {
  mMarkerId = markerId;
  mSerial = new SoftwareSerial(wifiModuleTX, wifiModuleRX);
  mSerial->begin(9600);

  //Wait for the esp module to connect. It will send a '0x99' byte when it is ready.
  byte it = 0;
  do {
    mSerial->write(0x99); delay(2);
    it = mSerial->read();
    delay(10);
    Serial.println(it);
  } while (it != 0x99);

  //At this point we know the ESP is ready for us to send shit.
  mSerial->write(OP_BEGIN);
  mSerial->write(teamType);
  mSerial->write(markerId >> 8);
  mSerial->write(markerId & 0xFF);
  mSerial->write(teamName);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();

  return receive(&missionSite);
}

bool VisionSystemClient::updateLocation() {
  mSerial->write(OP_LOCATION);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();

  return receive(&location);
}

void VisionSystemClient::mission(int type, int message) {
  mSerial->write(OP_MISSION);
  mSerial->write(type);
  mSerial->print(message);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

void VisionSystemClient::mission(int type, double message) {
  mSerial->write(OP_MISSION);
  mSerial->write(type);
  mSerial->print(message);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
}
void VisionSystemClient::mission(int type, char message) {
  mSerial->write(OP_MISSION);
  mSerial->write(type);
  mSerial->print(message);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

void VisionSystemClient::mission(int type, Coordinate& message) {
  mSerial->write(OP_MISSION);
  mSerial->write(type);
  mSerial->print(message.x);
  mSerial->print(',');
  mSerial->print(message.y);
  mSerial->print(',');
  mSerial->print(message.theta);
  mSerial->write(FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

int VisionSystemClient::MLGetPrediction() {
    mSerial->write(OP_ML_PREDICTION);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();

    byte buffer[2];
    while(!mSerial->available());
    buffer[0] = mSerial->read();
    while(!mSerial->available());
    buffer[1] = mSerial->read();
    return buffer[1] << 8 | buffer[0];
}

void VisionSystemClient::MLCaptureTrainingImage(const char * label) {
    //OP_ML_CAPTURE
    mSerial->write(OP_ML_CAPTURE);
    mSerial->print(label);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
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

void VisionSystemClient::readBytes(byte* buffer, int length) {
    auto start = millis();
    for(int i = 0; i < length; i++) {
        while(!mSerial->available()) {
            if(millis() - start > 100) {
                return;
            }
        }
        buffer[i] = mSerial->read();
    }
}

// A nice fancy faster function.
void VisionSystemClient::updateIfNeeded() {
    auto start = micros();
    if(millis() - lastUpdate < 50) return; // Don't check if we recently checked.
    lastUpdate = millis();
    mSerial->write(OP_CHECK);
    mSerial->flush();
    auto sent = micros();
    while(!mSerial->available()) {
        if(millis() - lastUpdate > 100) {
            return;
        }
    }
    auto received = micros();
    byte b = mSerial->read();
//    Serial.println(b);
    if(b == 0x00) return; //Zero means no update.
    if(b == 0x01) { // One means no marker found.
        location.x = -1;
        location.y = -1;
        location.theta = -1;
        visible = false;
        return;
    }
    // Two means marker found.
    // The response will be three ints, x, y, theta. X is a single byte representing 0-255, which is divided by 100 to get location.x
    // The response will be three ints, x, y, theta. Y is two bytes representing 0-65535, which is divided by 100 to get location.y
    // The response will be three ints, x, y, theta. Theta is two bytes, signed, representing -32768-32767, which is divided by 100 to get location.theta
    visible = true;
//    auto read_s = micros();
    byte buff[2];
    readBytes(buff, 1);
    location.x = float(buff[0]) / 100.0;
//    auto read_1 = micros();
    readBytes(buff, 2);
    location.y = float(buff[1] << 8 | buff[0]) / 100.0;
//    auto read_2 = micros();
    readBytes(buff, 2); //Remember this number is singed. Dereference buff to get the data
    int16_t intData = *((int16_t *) buff);
    location.theta = float(intData) / 100.0;
//    auto read_e = micros();

//    Serial.print(F("Done. Sent: "));
//    Serial.print(sent - start);
//    Serial.print(F(" Received: "));
//    Serial.print(received - sent);
//    Serial.print(F(" Read Total: "));
//    Serial.print(read_e - read_s);
//    Serial.print(F(" Read 1: "));
//    Serial.print(read_1 - read_s);
//    Serial.print(F(" Read 2: "));
//    Serial.print(read_2 - read_1);
//    Serial.print(F(" Read 3: "));
//    Serial.println(read_e - read_2);
}

float VisionSystemClient::getX() {
    updateIfNeeded();
    return location.x;
}

float VisionSystemClient::getY() {
    updateIfNeeded();
    return location.y;
}

float VisionSystemClient::getTheta() {
    updateIfNeeded();
    return location.theta;
}

bool VisionSystemClient::getVisibility() {
    updateIfNeeded();
    return visible;
}