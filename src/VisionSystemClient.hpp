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

class VisionSystemClient {
public:
    bool isConnected();
    // 0: Arduino connected to wifi module, but wifi module not connected to vision system
    // 1: Arduino + wifi module + vision system connected
    // 255: wifi module not responding
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

/**
 * Templated functions have to be in the header file
 **/
template <typename T>
void VisionSystemClient::print(T message) {
    if (mSerial == nullptr) return;
    maintainConnection();

    mSerial->write(OP_PRINT);
    mSerial->print(message);
    mSerial->write((byte) 0x00);
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
    mSerial->write((byte) 0x00);
    mSerial->write(FLUSH_SEQUENCE, 4);
    mSerial->flush();
}

#endif /* VisionSystemClient_hpp */