#include "VisionSystemClient.hpp"
#include <string.h>

static const uint32_t VS_SERIAL_BAUD       = 19200;
static const uint32_t VS_BEGIN_RETRY_MS    = 700;
static const uint32_t VS_STATE_POLL_MS     = 250;
static const uint32_t VS_BOOT_SETTLE_MS    = 25;
static const uint32_t VS_BEGIN_QUERY_MS    = 50;
static const uint32_t VS_RX_TIMEOUT_MS     = 100;

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

void VisionSystemClient::clearInput(uint16_t quietMs, uint16_t maxMs) {
    if (mSerial == nullptr) return;

    const unsigned long start = millis();
    unsigned long lastRx = millis();

    while ((millis() - start) < maxMs) {
        bool consumed = false;

        while (mSerial->available()) {
            mSerial->read();
            consumed = true;
            lastRx = millis();
        }

        if (!consumed && (millis() - lastRx) >= quietMs) {
            return;
        }
    }
}

byte VisionSystemClient::queryState(uint16_t timeoutMs) {
    if (mSerial == nullptr) return 0xFF;

    clearInput();
    mSerial->write(OP_IS_CONNECTED);
    mSerial->flush();

    const unsigned long start = millis();
    while ((millis() - start) < timeoutMs) {
        while (mSerial->available()) {
            int raw = mSerial->read();
            if (raw < 0) continue;

            byte b = (byte)raw;
            if (b == 0x00 || b == 0x01) {
                return b;
            }
        }
    }

    return 0xFF;
}

void VisionSystemClient::sendBeginPacket() {
    if (mSerial == nullptr || !mConfigValid) return;

    mSerial->write(OP_BEGIN);
    mSerial->write(mTeamType);
    mSerial->write((byte)(mMarkerId >> 8));
    mSerial->write((byte)(mMarkerId & 0xFF));
    mSerial->write((byte)(mRoomNumber >> 8));
    mSerial->write((byte)(mRoomNumber & 0xFF));
    mSerial->write((const uint8_t*)mTeamName, strlen(mTeamName));
    mSerial->write((byte)0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();

    mLastBeginSendMs = millis();
}

void VisionSystemClient::maintainConnection(bool aggressive) {
    if (mSerial == nullptr || !mConfigValid) return;

    const uint32_t now = millis();

    if (aggressive || (now - mLastStatePollMs) >= VS_STATE_POLL_MS) {
        mLastStatePollMs = now;
        mLastState = queryState();
    }

    if (mLastState != 0x01) {
        if (aggressive || mLastBeginSendMs == 0 || (now - mLastBeginSendMs) >= VS_BEGIN_RETRY_MS) {
            sendBeginPacket();
        }
    }
}

void VisionSystemClient::begin(const char* teamName, byte teamType, int markerId, int roomNumber, int wifiModuleTX, int wifiModuleRX) {
    mMarkerId = markerId;
    mRoomNumber = roomNumber;
    mTeamType = teamType;
    strncpy(mTeamName, teamName, sizeof(mTeamName) - 1);
    mTeamName[sizeof(mTeamName) - 1] = '\0';
    mConfigValid = true;

    location.x = -1;
    location.y = -1;
    location.theta = -1;
    visible = false;
    lastUpdate = 0;
    mLastBeginSendMs = 0;
    mLastStatePollMs = 0;
    mLastState = 0xFF;

    if (mSerial != nullptr) {
        delete mSerial;
        mSerial = nullptr;
    }

    mSerial = new SoftwareSerial(wifiModuleTX, wifiModuleRX);
    mSerial->begin(VS_SERIAL_BAUD);

    // IMPORTANT:
    // Do not block here. Just set up the link, clear startup garbage,
    // send the first begin packet, do one short state probe, and return.
    delay(VS_BOOT_SETTLE_MS);
    clearInput(5, 120);

    sendBeginPacket();
    mLastState = queryState(VS_BEGIN_QUERY_MS);
}

byte VisionSystemClient::state() {
    maintainConnection();
    return mLastState;
}

bool VisionSystemClient::isConnected() {
    maintainConnection();
    return mLastState == 0x01;
}

void VisionSystemClient::mission(int type, int message) {
    if(mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, double message) {
    if(mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, char message) {
    if(mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, Coordinate message) {
    if(mSerial == nullptr) return;
    maintainConnection();

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
    maintainConnection();
    clearInput();

    mSerial->write(OP_ML_PREDICTION);
    mSerial->write(model_index);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();

    byte buffer[2] = {0xFF, 0xFF};

    unsigned long start = millis();
    while(!mSerial->available()) {
        if(millis() - start > VS_RX_TIMEOUT_MS) {
            return -1;
        }
    }
    buffer[0] = mSerial->read();

    start = millis();
    while(!mSerial->available()) {
        if(millis() - start > VS_RX_TIMEOUT_MS) {
            return -1;
        }
    }
    buffer[1] = mSerial->read();

    return (buffer[1] << 8) | buffer[0];
}

void VisionSystemClient::readBytes(byte* buffer, int length) {
    auto start = millis();
    for(int i = 0; i < length; i++) {
        while(!mSerial->available()) {
            if(millis() - start > VS_RX_TIMEOUT_MS) {
                return;
            }
        }
        buffer[i] = mSerial->read();
    }
}

void VisionSystemClient::updateIfNeeded() {
    if(mSerial == nullptr) return;
    maintainConnection();
    if(mLastState != 0x01) return;

    if(millis() - lastUpdate < 50) return;
    lastUpdate = millis();

    clearInput();
    mSerial->write(OP_CHECK);
    mSerial->flush();

    while(!mSerial->available()) {
        if(millis() - lastUpdate > VS_RX_TIMEOUT_MS) {
            return;
        }
    }

    byte b = mSerial->read();
    if(b == 0x00) return;
    if(b == 0x01) {
        location.x = -1;
        location.y = -1;
        location.theta = -1;
        visible = false;
        return;
    }
    if(b != 0x02) return;

    visible = true;
    byte buff[2];

    readBytes(buff, 1);
    location.y = float(buff[0]) / 100.0f;

    readBytes(buff, 2);
    location.x = float((buff[1] << 8) | buff[0]) / 100.0f;

    readBytes(buff, 2);
    int16_t intData = *((int16_t *) buff);
    location.theta = float(intData) / 100.0f;
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