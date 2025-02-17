#include "VisionSystemClient.hpp"

Coordinate::Coordinate() {
  this->x = 0;
    this->y = 0;
    this->theta = 0;
}

Coordinate::Coordinate(double x, double y) {
    this->x = x;
    this->y = y;
    this->theta = 0;
}

Coordinate::Coordinate(double x, double y, double theta) {
    this->x = x;
    this->y = y;
    this->theta = theta;
}

void VisionSystemClient::begin(const char* teamName, byte teamType, int markerId, int roomNumber, int wifiModuleTX, int wifiModuleRX) {
    mMarkerId = markerId;
    if (mSerial != nullptr) delete mSerial;
    mSerial = new SoftwareSerial(wifiModuleTX, wifiModuleRX);
    mSerial->begin(57600);

    //Wait for the esp module to connect. It will send a '0x00' byte when it is ready but not connect to WiFi.
    while(state() != 0x00 && state() != 0x01) {
        // Serial.print("Waiting for 0x00 or 0x01, state=");
        // Serial.println(state());
        delay(50);
    }

    //At this point we know the ESP is ready for us to send shtuff.
    mSerial->write(OP_BEGIN); // Opcode
    mSerial->write(teamType); // Team Type, one byte
    mSerial->write(markerId >> 8); // Marker ID high
    mSerial->write(markerId & 0xFF); // Marker ID low
    mSerial->write(roomNumber >> 8); // Room Number high
    mSerial->write(roomNumber & 0xFF); // Room Number low
    mSerial->write(teamName, strlen(teamName)); // Team Name
    // Serial.println("Sending team name");
    // Serial.println(teamName);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();


    while(state() != 0x01) {
        // Serial.print("Waiting for 0x01, state=");
        // Serial.println(state());
        delay(50);
    }
}

byte VisionSystemClient::state() {
    if (mSerial == nullptr) return 0xFF;
    while(mSerial->available()) mSerial->read(); // Remove bytes from incoming buffer.
    mSerial->write(OP_IS_CONNECTED);
    unsigned long start = millis();
    // While we have been waiting less than 10ms and there are no bytes available.
    while (millis() - start < 10 && mSerial->available() == 0); //Do nothing.
    //    Serial.println(millis() - start);
    return mSerial->read();
}

bool VisionSystemClient::isConnected() {
    return state() == 0x01;
}

void VisionSystemClient::mission(int type, int message) {
    if(mSerial == nullptr) return;
    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, double message) {
    if(mSerial == nullptr) return;
    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, char message) {
    if(mSerial == nullptr) return;
    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, Coordinate message) {
    if(mSerial == nullptr) return;
    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message.x);
    mSerial->print(',');
    mSerial->print(message.y);
    mSerial->print(',');
    mSerial->print(message.theta);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

int VisionSystemClient::MLGetPrediction(int model_index) {
    if(mSerial == nullptr) return -1;
    mSerial->write(OP_ML_PREDICTION);
    mSerial->write(model_index);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();

    delay(1);

    while (mSerial->available()) {
        mSerial->read();
    }

    byte buffer[2];
    while(!mSerial->available());
    buffer[0] = mSerial->read();
    while(!mSerial->available());
    buffer[1] = mSerial->read();
    return (buffer[1] << 8) | buffer[0];
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
        // Serial.print(buffer[i], HEX);
        // Serial.print(" ");
    }
}

// A nice fancy faster function.
void VisionSystemClient::updateIfNeeded() {
    if(mSerial == nullptr) return;
    if(millis() - lastUpdate < 50) return; // Don't check if we recently checked.
    lastUpdate = millis();
    while(mSerial->available()) mSerial->read(); // Remove bytes from incoming buffer.
//    auto start = micros();
    mSerial->write(OP_CHECK);
    mSerial->flush();
//    auto sent = micros();
    while(!mSerial->available()) {
        if(millis() - lastUpdate > 100) {
            return;
        }
    }
//    auto received = micros();
    byte b = mSerial->read();
//    Serial.print(b, HEX);
//    Serial.print(" ");
    if(b == 0x00) return; //Zero means no update.
    if(b == 0x01) { // One means no marker found.
        location.x = -1;
        location.y = -1;
        location.theta = -1;
        visible = false;
        return;
    }
    if (b != 0x02) return; // All other invalid values should be returned.

    // The response will be three ints, x, y, theta.
    // X is a single byte representing 0-255, which is divided by 100 to get location.x
    // Y is two bytes representing 0-65535, which is divided by 100 to get location.y
    // Theta is two bytes, signed, representing -32768-32767, which is divided by 100 to get location.theta
    visible = true;
//    auto read_s = micros();
    byte buff[2];

    readBytes(buff, 1);
    location.y = float(buff[0]) / 100.0;

    readBytes(buff, 2);
    location.x = float(buff[1] << 8 | buff[0]) / 100.0;
//    auto read_1 = micros();

    readBytes(buff, 2); //Remember this number is singed. Dereference buff to get the data
//    auto read_2 = micros();

    int16_t intData = *((int16_t *) buff);
    location.theta = float(intData) / 100.0;
//    auto read_e = micros();

//    Serial.print(F("Sent: "));
//    Serial.print(sent - start);
//    Serial.print(F("us Received: "));
//    Serial.print(received - sent);
//    Serial.print(F("us Read Total: "));
//    Serial.print(read_e - read_s);
//    Serial.print(F("us Read x: "));
//    Serial.print(read_1 - read_s);
//    Serial.print(F("us Read y: "));
//    Serial.print(read_2 - read_1);
//    Serial.print(F("us Read theta: "));
//    Serial.print(read_e - read_2);
//    Serial.print(F("us Total: "));
//    Serial.println(read_e - start);
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

bool VisionSystemClient::isVisible() {
    updateIfNeeded();
    return visible;
}

bool VisionSystemClient::updateLocation() {
    updateIfNeeded();
    return true;
}
