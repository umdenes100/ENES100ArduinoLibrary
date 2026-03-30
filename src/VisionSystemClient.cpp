#include "VisionSystemClient.hpp"
#include <string.h>

static const uint32_t VS_SERIAL_BAUD       = 19200;
static const uint32_t VS_BEGIN_RETRY_MS    = 700;
static const uint32_t VS_STATE_POLL_MS     = 250;
static const uint32_t VS_BOOT_SETTLE_MS    = 25;
static const uint32_t VS_BEGIN_QUERY_MS    = 50;
static const uint32_t VS_RX_TIMEOUT_MS     = 100;

static const char* wsEventName(uint8_t e) {
    switch (e) {
        case 1: return "open";
        case 2: return "close";
        case 3: return "ping";
        case 4: return "pong";
        default: return "none";
    }
}

static const char* wifiDiscReasonName(uint8_t r) {
    switch (r) {
        case 0:   return "none";
        case 2:   return "auth_expire";
        case 4:   return "assoc_expire";
        case 8:   return "assoc_leave";
        case 15:  return "4way_timeout";
        case 200: return "beacon_timeout";
        case 201: return "no_ap_found";
        case 202: return "auth_fail";
        case 203: return "assoc_fail";
        case 204: return "handshake_timeout";
        case 253: return "dhcp_timeout";
        case 254: return "join_timeout";
        default:  return "other";
    }
}

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

bool VisionSystemClient::readByteTimeout(byte& out, uint16_t timeoutMs) {
    if (mSerial == nullptr) return false;

    const unsigned long start = millis();
    while (!mSerial->available()) {
        if ((millis() - start) > timeoutMs) {
            return false;
        }
    }

    int raw = mSerial->read();
    if (raw < 0) return false;
    out = (byte)raw;
    return true;
}

bool VisionSystemClient::readExact(byte* buffer, int length, uint16_t timeoutMs) {
    for (int i = 0; i < length; i++) {
        if (!readByteTimeout(buffer[i], timeoutMs)) {
            return false;
        }
    }
    return true;
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

bool VisionSystemClient::debugStatus(Enes100DebugStatus& out) {
    out = Enes100DebugStatus();

    if (mSerial == nullptr) return false;
    maintainConnection();

    clearInput();
    mSerial->write(OP_DEBUG_STATUS);
    mSerial->flush();

    byte header[25];
    if (!readExact(header, 25, VS_RX_TIMEOUT_MS)) {
        return false;
    }

    if (header[0] != 0xA5 || header[1] != 0x02) {
        return false;
    }

    const byte flags = header[2];
    out.wifiConnected      = (flags & 0x01) != 0;
    out.wsStarted          = (flags & 0x02) != 0;
    out.wsConnected        = (flags & 0x04) != 0;
    out.hasBegin           = (flags & 0x08) != 0;
    out.everWsConnected    = (flags & 0x10) != 0;
    out.lastConnectCallOk  = (flags & 0x20) != 0;
    out.routeIsFallback    = (flags & 0x40) != 0;
    out.wifiJoinInProgress = (flags & 0x80) != 0;

    out.wifiStatus         = header[3];
    out.room               = (uint16_t)(((uint16_t)header[4] << 8) | header[5]);
    out.routeIndex         = header[6];
    out.routeCount         = header[7];
    out.connectAttempts    = (uint16_t)(((uint16_t)header[9] << 8) | header[8]);
    out.openEvents         = header[10];
    out.closeEvents        = header[11];
    out.lastEvent          = header[12];
    out.rssi               = (int8_t)header[13];
    out.localIp[0]         = header[14];
    out.localIp[1]         = header[15];
    out.localIp[2]         = header[16];
    out.localIp[3]         = header[17];
    out.lastDisconnectReason = header[18];
    out.wifiBeginCount     = (uint16_t)(((uint16_t)header[20] << 8) | header[19]);
    out.wifiGotIpCount     = (uint16_t)(((uint16_t)header[22] << 8) | header[21]);
    out.wifiJoinAgeSec     = header[23];

    byte urlLen = header[24];
    if (urlLen >= sizeof(out.currentUrl)) {
        urlLen = sizeof(out.currentUrl) - 1;
    }

    if (urlLen > 0) {
        if (!readExact((byte*)out.currentUrl, urlLen, VS_RX_TIMEOUT_MS)) {
            return false;
        }
    }
    out.currentUrl[urlLen] = '\0';
    out.valid = true;
    return true;
}

void VisionSystemClient::debugDump(Stream& out) {
    Enes100DebugStatus d;
    if (!debugStatus(d)) {
        out.println("ENES100 debug read failed");
        return;
    }

    out.println("----- ENES100 DEBUG -----");
    out.print("state() = ");
    out.println((int)mLastState);

    out.print("wifiConnected = ");
    out.println(d.wifiConnected ? "true" : "false");
    out.print("wifiJoinInProgress = ");
    out.println(d.wifiJoinInProgress ? "true" : "false");
    out.print("wsStarted = ");
    out.println(d.wsStarted ? "true" : "false");
    out.print("wsConnected = ");
    out.println(d.wsConnected ? "true" : "false");
    out.print("hasBegin = ");
    out.println(d.hasBegin ? "true" : "false");
    out.print("everWsConnected = ");
    out.println(d.everWsConnected ? "true" : "false");
    out.print("lastConnectCallOk = ");
    out.println(d.lastConnectCallOk ? "true" : "false");
    out.print("routeIsFallback = ");
    out.println(d.routeIsFallback ? "true" : "false");

    out.print("wifiStatus = ");
    out.println((int)d.wifiStatus);
    out.print("room = ");
    out.println((int)d.room);

    out.print("route = ");
    out.print((int)d.routeIndex + 1);
    out.print("/");
    out.println((int)d.routeCount);

    out.print("connectAttempts = ");
    out.println((int)d.connectAttempts);
    out.print("openEvents = ");
    out.println((int)d.openEvents);
    out.print("closeEvents = ");
    out.println((int)d.closeEvents);
    out.print("lastEvent = ");
    out.println(wsEventName(d.lastEvent));

    out.print("RSSI = ");
    out.println((int)d.rssi);

    out.print("localIP = ");
    out.print((int)d.localIp[0]); out.print(".");
    out.print((int)d.localIp[1]); out.print(".");
    out.print((int)d.localIp[2]); out.print(".");
    out.println((int)d.localIp[3]);

    out.print("wifiBeginCount = ");
    out.println((int)d.wifiBeginCount);
    out.print("wifiGotIpCount = ");
    out.println((int)d.wifiGotIpCount);
    out.print("wifiJoinAgeSec = ");
    out.println((int)d.wifiJoinAgeSec);
    out.print("lastDisconnectReason = ");
    out.print((int)d.lastDisconnectReason);
    out.print(" (");
    out.print(wifiDiscReasonName(d.lastDisconnectReason));
    out.println(")");

    out.print("currentUrl = ");
    out.println(d.currentUrl[0] ? d.currentUrl : "(empty)");

    out.println("-------------------------");
}

void VisionSystemClient::mission(int type, int message) {
    if(mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte)0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, double message) {
    if(mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte)0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

void VisionSystemClient::mission(int type, char message) {
    if(mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_MISSION);
    mSerial->write(type);
    mSerial->print(message);
    mSerial->write((byte)0x00);
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
    mSerial->write((byte)0x00);
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

    // changing VS_RX_TIMEOUT_MS to accomodate prediction time
    static const uint32_t VS_ML_RX_TIMEOUT_MS  = 5000;
    const unsigned long start = millis();
    int count = 0;
    while(count < 2) {
        if(mSerial->available()) {
            buffer[count++] = mSerial->read();
        }
    
        if(millis() - start > VS_ML_RX_TIMEOUT_MS) {
            return -1;
        }
    }

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
