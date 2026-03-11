#ifndef VisionSystemClient_hpp
#define VisionSystemClient_hpp

#include "Arduino.h"
#include "SoftwareSerial.h"

// Transmission opcodes
#define OP_BEGIN            0x1
#define OP_PRINT            0x2
#define OP_CHECK            0x3
#define OP_MISSION          0x4
#define OP_ML_PREDICTION    0x5
#define OP_ML_CAPTURE       0x6
#define OP_IS_CONNECTED     0x7
#define OP_DEBUG_STATUS     0x8

const byte FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC};

class Coordinate {
public:
    Coordinate();
    Coordinate(double x, double y);
    Coordinate(double x, double y, double theta);

    double x;
    double y;
    double theta;
};

struct Enes100DebugStatus {
    bool valid = false;

    bool wifiConnected = false;
    bool wsStarted = false;
    bool wsConnected = false;
    bool hasBegin = false;
    bool everWsConnected = false;
    bool lastConnectCallOk = false;
    bool routeIsFallback = false;

    uint8_t wifiStatus = 0;
    uint16_t room = 0;
    uint8_t routeIndex = 0;
    uint8_t routeCount = 0;
    uint16_t connectAttempts = 0;
    uint8_t openEvents = 0;
    uint8_t closeEvents = 0;
    uint8_t lastEvent = 0;
    int8_t rssi = 0;

    uint8_t localIp[4] = {0, 0, 0, 0};
    char currentUrl[64] = {0};
};

class VisionSystemClient {
public:
    bool isConnected();
    byte state();
    void begin(const char* teamName, byte teamType, int markerId, int roomNumber, int wifiModuleTX, int wifiModuleRX);

    float getX();
    float getY();
    float getTheta();
    bool isVisible();
    bool updateLocation();

    void mission(int type, int message);
    void mission(int type, double message);
    void mission(int type, char message);
    void mission(int type, Coordinate message);
    int MLGetPrediction(int);
    void MLCaptureTrainingImage(const char * label);

    bool debugStatus(Enes100DebugStatus& out);
    void debugDump(Stream& out);

    template <typename T>
    void print(T message);
    template <typename T>
    void println(T message);

    Coordinate location;

private:
    bool receive(Coordinate* coordinate = NULL);
    bool visible = false;
    void updateIfNeeded();
    void readBytes(byte* buffer, int length);

    void clearInput(uint16_t quietMs = 4, uint16_t maxMs = 40);
    byte queryState(uint16_t timeoutMs = 30);
    void sendBeginPacket();
    void maintainConnection(bool aggressive = false);

    bool readByteTimeout(byte& out, uint16_t timeoutMs);
    bool readExact(byte* buffer, int length, uint16_t timeoutMs);

    uint32_t lastUpdate = 0;
    uint32_t mLastBeginSendMs = 0;
    uint32_t mLastStatePollMs = 0;
    byte mLastState = 0xFF;

    int mMarkerId = 0;
    int mRoomNumber = 0;
    byte mTeamType = 0;
    char mTeamName[64] = {0};
    bool mConfigValid = false;

    SoftwareSerial* mSerial = nullptr;
};

template <typename T>
void VisionSystemClient::print(T message) {
    if (mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_PRINT);
    mSerial->print(message);
    mSerial->write((byte)0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

template <typename T>
void VisionSystemClient::println(T message) {
    if (mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_PRINT);
    mSerial->print(message);
    mSerial->write('\n');
    mSerial->write((byte)0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

#endif /* VisionSystemClient_hpp */