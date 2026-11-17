#if defined(ARDUINO_UNOR4_WIFI)

// UNO R4 WiFi "camera as WiFi module" link. This file mirrors the classic
// VisionSystemClient implementation at the top of Enes100.cpp: same opcodes,
// packet layouts, timing and retries. Only the serial port type differs
// (HardwareSerial instead of SoftwareSerial). Keep the two in sync.

#include "Enes100R4Camera.h"
#include "Enes100R4.h"
#include <string.h>

namespace {

// Transmission opcodes (same values as VisionSystemClient.hpp).
const byte CAM_OP_BEGIN         = 0x1;
const byte CAM_OP_PRINT         = 0x2;
const byte CAM_OP_CHECK         = 0x3;
const byte CAM_OP_MISSION       = 0x4;
const byte CAM_OP_ML_PREDICTION = 0x5;
const byte CAM_OP_IS_CONNECTED  = 0x7;

const byte CAM_FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC};

const uint32_t CAM_SERIAL_BAUD      = 19200;
const uint32_t CAM_BEGIN_RETRY_MS   = 700;
const uint32_t CAM_STATE_POLL_MS    = 250;
const uint32_t CAM_BOOT_SETTLE_MS   = 25;
const uint32_t CAM_BEGIN_QUERY_MS   = 50;
const uint32_t CAM_RX_TIMEOUT_MS    = 100;
const uint32_t CAM_ML_RX_TIMEOUT_MS = 5000;

// On the UNO R4 every Serial1.begin() permanently consumes interrupt slots,
// so a port is opened only the first time it is handed to begin().
HardwareSerial* sOpenedPort = nullptr;

} // namespace

// Drop any stale bytes from the camera (for example a late ML reply) so the
// next reply we read belongs to the command we are about to send.
void Enes100R4CameraLink::clearInput(uint16_t quietMs, uint16_t maxMs) {
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

// Sends the 0x07 poll. Returns 0x01 once the camera has our begin packet,
// 0x00 if it answered but has not got it yet, 0xFF if it did not answer
// (wiring, power, switch in training position, or camera still booting).
byte Enes100R4CameraLink::queryState(uint16_t timeoutMs) {
  if (mSerial == nullptr) return 0xFF;

  clearInput();
  mSerial->write(CAM_OP_IS_CONNECTED);
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

// Begin packet: 0x01 [teamType] [marker hi][marker lo] [room hi][room lo]
// [team name] 0x00 FF FE FD FC. The camera joins the room's server after this.
void Enes100R4CameraLink::sendBeginPacket() {
  if (mSerial == nullptr || !mConfigValid) return;

  mSerial->write(CAM_OP_BEGIN);
  mSerial->write(mTeamType);
  mSerial->write((byte)(mMarkerId >> 8));
  mSerial->write((byte)(mMarkerId & 0xFF));
  mSerial->write((byte)(mRoomNumber >> 8));
  mSerial->write((byte)(mRoomNumber & 0xFF));
  mSerial->write((const uint8_t*)mTeamName, strlen(mTeamName));
  mSerial->write((byte)0x00);
  mSerial->write(CAM_FLUSH_SEQUENCE, 4);
  mSerial->flush();

  mLastBeginSendMs = millis();
}

// Called at the start of every public call, like the classic library: polls
// the camera at most every 250 ms and resends begin every 700 ms until the
// camera reports connected (this is how a camera reboot recovers by itself).
void Enes100R4CameraLink::maintainConnection(bool aggressive) {
  if (mSerial == nullptr || !mConfigValid) return;

  const uint32_t now = millis();

  if (aggressive || (now - mLastStatePollMs) >= CAM_STATE_POLL_MS) {
    mLastStatePollMs = now;
    mLastState = queryState();
  }

  if (mLastState != 0x01) {
    if (aggressive || mLastBeginSendMs == 0 || (now - mLastBeginSendMs) >= CAM_BEGIN_RETRY_MS) {
      sendBeginPacket();
    }
  }
}

void Enes100R4CameraLink::begin(const char* teamName, byte teamType, int markerId, int roomNumber,
                                HardwareSerial& port, Coordinate* location) {
  mMarkerId = markerId;
  mRoomNumber = roomNumber;
  mTeamType = teamType;
  strncpy(mTeamName, teamName ? teamName : "", sizeof(mTeamName) - 1);
  mTeamName[sizeof(mTeamName) - 1] = '\0';
  mConfigValid = true;

  mLocation = location;
  mX = mY = mTheta = -1;
  if (mLocation != nullptr) {
    mLocation->x = -1;
    mLocation->y = -1;
    mLocation->theta = -1;
  }
  visible = false;
  lastUpdate = 0;
  mLastBeginSendMs = 0;
  mLastStatePollMs = 0;
  mLastState = 0xFF;

  // Open the UART only once (R4 interrupt-slot quirk). A second begin() call
  // just resets the state above and resends the begin packet.
  mSerial = &port;
  if (sOpenedPort != &port) {
    port.begin(CAM_SERIAL_BAUD);
    sOpenedPort = &port;
  }

  delay(CAM_BOOT_SETTLE_MS);
  clearInput(5, 120);

  sendBeginPacket();
  mLastState = queryState(CAM_BEGIN_QUERY_MS);
}

byte Enes100R4CameraLink::state() {
  maintainConnection();
  return mLastState;
}

bool Enes100R4CameraLink::isConnected() {
  maintainConnection();
  return mLastState == 0x01;
}

void Enes100R4CameraLink::print(const char* message, bool newline) {
  if (mSerial == nullptr) return;
  maintainConnection();

  mSerial->write(CAM_OP_PRINT);
  mSerial->print(message ? message : "");
  if (newline) mSerial->write('\n');
  mSerial->write((byte)0x00);
  mSerial->write(CAM_FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

void Enes100R4CameraLink::mission(int type, int message) {
  if (mSerial == nullptr) return;
  maintainConnection();

  mSerial->write(CAM_OP_MISSION);
  mSerial->write((byte)type);
  mSerial->print(message);
  mSerial->write((byte)0x00);
  mSerial->write(CAM_FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

void Enes100R4CameraLink::mission(int type, double message) {
  if (mSerial == nullptr) return;
  maintainConnection();

  mSerial->write(CAM_OP_MISSION);
  mSerial->write((byte)type);
  mSerial->print(message);
  mSerial->write((byte)0x00);
  mSerial->write(CAM_FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

void Enes100R4CameraLink::mission(int type, char message) {
  if (mSerial == nullptr) return;
  maintainConnection();

  mSerial->write(CAM_OP_MISSION);
  mSerial->write((byte)type);
  mSerial->print(message);
  mSerial->write((byte)0x00);
  mSerial->write(CAM_FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

void Enes100R4CameraLink::mission(int type, double x, double y, double theta) {
  if (mSerial == nullptr) return;
  maintainConnection();

  mSerial->write(CAM_OP_MISSION);
  mSerial->write((byte)type);
  mSerial->print(x);
  mSerial->print(',');
  mSerial->print(y);
  mSerial->print(',');
  mSerial->print(theta);
  mSerial->write((byte)0x00);
  mSerial->write(CAM_FLUSH_SEQUENCE, 4);
  mSerial->flush();
}

int Enes100R4CameraLink::MLGetPrediction(int modelIndex) {
  if (mSerial == nullptr) return -1;
  maintainConnection();
  clearInput();

  // ML packet: 0x05 [model index] FF FE FD FC. The camera takes a photo, the
  // server runs the team's model, and the camera writes back 2 bytes
  // (int16, little-endian). No reply within 5 s returns -1.
  mSerial->write(CAM_OP_ML_PREDICTION);
  mSerial->write((byte)modelIndex);
  mSerial->write(CAM_FLUSH_SEQUENCE, 4);
  mSerial->flush();

  byte buffer[2] = {0xFF, 0xFF};

  const unsigned long start = millis();
  int count = 0;
  while (count < 2) {
    if (mSerial->available()) {
      buffer[count++] = mSerial->read();
    }

    if (millis() - start > CAM_ML_RX_TIMEOUT_MS) {
      return -1;
    }
  }

  // The camera sends a 16-bit little-endian value. int is 32 bits on the R4,
  // so go through int16_t to keep negative results negative, as on the Uno.
  return (int16_t)(((uint16_t)buffer[1] << 8) | buffer[0]);
}

void Enes100R4CameraLink::readBytes(byte* buffer, int length) {
  auto start = millis();
  for (int i = 0; i < length; i++) {
    while (!mSerial->available()) {
      if (millis() - start > CAM_RX_TIMEOUT_MS) {
        return;
      }
    }
    buffer[i] = mSerial->read();
  }
}

void Enes100R4CameraLink::updateIfNeeded() {
  if (mSerial == nullptr) return;
  maintainConnection();
  if (mLastState != 0x01) return;

  if (millis() - lastUpdate < 50) return;
  lastUpdate = millis();

  clearInput();
  mSerial->write(CAM_OP_CHECK);
  mSerial->flush();

  while (!mSerial->available()) {
    if (millis() - lastUpdate > CAM_RX_TIMEOUT_MS) {
      return;
    }
  }

  // Location reply: 0x00 = no new data (keep the old values),
  // 0x01 = marker not visible, 0x02 = visible, followed by
  // y (u8), x (u16 LE), theta (i16 LE), all in hundredths.
  byte b = mSerial->read();
  if (b == 0x00) return;
  if (b == 0x01) {
    mX = mY = mTheta = -1;
    visible = false;
  } else if (b == 0x02) {
    visible = true;
    byte buff[2];

    readBytes(buff, 1);
    mY = float(buff[0]) / 100.0f;

    readBytes(buff, 2);
    mX = float((uint16_t)(((uint16_t)buff[1] << 8) | buff[0])) / 100.0f;

    readBytes(buff, 2);
    int16_t intData = (int16_t)(((uint16_t)buff[1] << 8) | buff[0]);
    mTheta = float(intData) / 100.0f;
  } else {
    return;
  }

  if (mLocation != nullptr) {
    mLocation->x = mX;
    mLocation->y = mY;
    mLocation->theta = mTheta;
  }
}

float Enes100R4CameraLink::getX() {
  updateIfNeeded();
  return mX;
}

float Enes100R4CameraLink::getY() {
  updateIfNeeded();
  return mY;
}

float Enes100R4CameraLink::getTheta() {
  updateIfNeeded();
  return mTheta;
}

bool Enes100R4CameraLink::isVisible() {
  updateIfNeeded();
  return visible;
}

bool Enes100R4CameraLink::updateLocation() {
  updateIfNeeded();
  return true;
}

// Local summary only: the camera firmware does not implement the 0x08 debug
// opcode, so nothing extra is sent to the camera here.
void Enes100R4CameraLink::debugDump(Stream& out) {
  maintainConnection();
  out.println("----- ENES100 R4 CAMERA-MODULE DEBUG -----");
  out.print("team = "); out.println(mTeamName);
  out.print("marker = "); out.println(mMarkerId);
  out.print("room = "); out.println(mRoomNumber);
  out.print("state() = "); out.print((int)mLastState);
  out.println(mLastState == 0x01 ? " (connected)"
              : mLastState == 0x00 ? " (camera responding, not connected yet)"
                                   : " (no response from camera)");
  out.print("visible = "); out.println(visible ? "true" : "false");
  out.println("------------------------------------------");
}

#endif // ARDUINO_UNOR4_WIFI
