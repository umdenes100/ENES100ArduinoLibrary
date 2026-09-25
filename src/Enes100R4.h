#pragma once

#include <Arduino.h>

class WiFiClient;
class WebSocketClient;

// -----------------------------------------------------------------------------
// ENES100 mission / message constants
// Kept compatible with the official ENES100 Arduino library.
// -----------------------------------------------------------------------------

#define CRASH_SITE 0
#define DATA       1
#define MATERIAL   2
#define FIRE       3
#define WATER      4
#define SEED       5
#define HYDROGEN   6

// Crash Site
#define DIRECTION  0
#define LENGTH     1
#define HEIGHT     2
#define POS_X      0
#define NEG_X      1
#define POS_Y      2
#define NEG_Y      3
// Official-library aliases retained for source compatibility.
#define NORMAL_X   0
#define NORMAL_Y   1

// Data
#define CYCLE          0
#define MAGNETISM      1
#define MAGNETIC       0
#define NOT_MAGNETIC   1

// Material
#define WEIGHT         0
#define MATERIAL_TYPE  1
#define HEAVY          0
#define MEDIUM         1
#define LIGHT          2
#define FOAM           0
#define PLASTIC        1

// Fire
#define NUM_CANDLES    0
#define TOPOGRAPHY     1
#define TOP_A          0
#define TOP_B          1
#define TOP_C          2

// Water
#define DEPTH               0
#define WATER_TYPE          1
#define FRESH_UNPOLLUTED    0
#define FRESH_POLLUTED      1
#define SALT_UNPOLLUTED     2
#define SALT_POLLUTED       3

// Seed
#define LOCATION       0
#define BOTH           0
#define NEITHER        1
#define ADJACENT       2
#define DIAGONAL       3

// Hydrogen
#define VOLTAGE_OUTPUT 0
#define LED_COLOR      1
#define WHITE          0
#define RED            1
#define YELLOW         2
#define GREEN          3
#define BLUE           4
#define VOLTAGE_1      1
#define VOLTAGE_2      2
#define VOLTAGE_3      3
#define VOLTAGE_4      4
#define VOLTAGE_5      5

class Coordinate {
public:
  Coordinate() : x(0), y(0), theta(0) {}
  Coordinate(double x_, double y_) : x(x_), y(y_), theta(0) {}
  Coordinate(double x_, double y_, double theta_) : x(x_), y(y_), theta(theta_) {}

  double x;
  double y;
  double theta;
};

struct Enes100R4DebugStatus {
  bool profileMatched = false;
  bool wifiConnected = false;
  bool wsConnected = false;
  bool taskRunning = false;
  uint8_t wifiStatus = 0;
  uint16_t room = 0;
  int32_t rssi = 0;
  uint32_t droppedMessages = 0;
  char hostname[11] = {0};
  char mac[18] = {0};
  char server[16] = {0};
};

class VisionSystemClientR4 {
public:
  VisionSystemClientR4();

  // Native UNO R4 WiFi begin(): no TX/RX pins are needed.
  bool begin(const char* teamName, byte teamType, int markerId, int roomNumber);

  // Compatibility overload for sketches written for the official ESP8266
  // library. wifiModuleTX/wifiModuleRX are intentionally ignored on UNO R4.
  void begin(const char* teamName,
             byte teamType,
             int markerId,
             int roomNumber,
             int wifiModuleTX,
             int wifiModuleRX);

  bool isConnected() const;
  byte state() const;

  float getX() const;
  float getY() const;
  float getTheta() const;
  bool isVisible() const;
  bool updateLocation() const;

  void mission(int type, int message);
  void mission(int type, double message);
  void mission(int type, char message);
  void mission(int type, Coordinate message);

  // There is no camera attached to the UNO R4 WiFi itself. Prediction is
  // therefore unavailable; capture still sends the server-side capture opcode.
  int MLGetPrediction(int modelIndex);
  void MLCaptureTrainingImage(const char* label);

  template <typename... Args>
  void print(const Args&... args) {
    String message;
    message.reserve(96);
    appendArgs(message, args...);
    enqueueUserMessage(message, false);
  }

  template <typename... Args>
  void println(const Args&... args) {
    String message;
    message.reserve(96);
    appendArgs(message, args...);
    enqueueUserMessage(message, true);
  }

  void println();

  // UNO R4 specific helpers.
  bool deviceKnown() const;
  const char* hostname() const;
  String macAddress() const;
  const char* serverAddress() const;
  uint32_t droppedMessages() const;
  bool debugStatus(Enes100R4DebugStatus& out) const;
  void debugDump(Stream& out) const;
  void setDebug(bool enabled);

  Coordinate location;

private:
  static constexpr uint8_t TX_QUEUE_DEPTH = 8;
  static constexpr size_t TX_TEXT_SIZE = 220;

  enum TxKind : uint8_t { TX_PRINT = 0, TX_CAPTURE = 1 };

  struct TxItem {
    uint8_t kind;
    char text[TX_TEXT_SIZE];
  };

  // Template helpers must live in the header.
  static void appendArgs(String&) {}

  template <typename T, typename... Rest>
  static void appendArgs(String& out, const T& first, const Rest&... rest) {
    if (out.length() > 0) out += ' ';
    appendOne(out, first);
    appendArgs(out, rest...);
  }

  static void appendOne(String& out, const String& value) { out += value; }
  static void appendOne(String& out, const char* value) { out += (value ? value : ""); }
  static void appendOne(String& out, char value) { out += value; }
  static void appendOne(String& out, unsigned char value) { out += String((unsigned int)value); }
  static void appendOne(String& out, signed char value) { out += String((int)value); }

  template <typename T>
  static void appendOne(String& out, const T& value) { out += String(value); }

  bool configure(const char* teamName, byte teamType, int markerId, int roomNumber);
  bool startBackgroundTask();
  static void taskThunk(void* arg);
  void taskLoop();

  bool discoverIdentity();
  bool connectWifi();
  bool connectWebSocket();
  void disconnectWebSocket();
  void chooseServerForRoom();

  bool sendText(const char* text);
  bool sendBeginPacket();
  bool sendArucoRequest();
  bool sendPing(const char* status);
  bool sendPrintPacket(const char* text);
  bool sendCapturePacket(const char* label);

  void handleIncoming(const String& message);
  void handleAruco(const String& message);
  void handlePing(const String& message);

  void enqueueUserMessage(const String& message, bool newline);
  bool enqueueRaw(const char* message, uint8_t kind = TX_PRINT);
  bool popRaw(char* out, size_t outLen, uint8_t& kind);

  void sendMissionValue(int type, const String& rawValue);
  String formatMission(int type, const String& rawValue) const;

  static const char* teamTypeToString(byte teamType);
  static byte teamTypeFromString(const char* value);
  static bool extractFloat(const String& json, const char* key, float& out);
  static bool extractBool(const String& json, const char* key, bool& out);
  static bool extractQuoted(const String& json, const char* key, String& out);
  static String jsonEscape(const char* input);
  static String jsonEscape(const String& input);
  static void formatMac(const uint8_t mac[6], char out[18]);
  static bool sameMac(const uint8_t a[6], const uint8_t b[6]);

  void debug(const char* message) const;
  void debugValue(const char* label, const char* value) const;

private:
  char _teamName[48];
  byte _teamType;
  int _markerId;
  uint16_t _room;

  volatile bool _configured;
  volatile bool _taskRunning;
  volatile bool _profileMatched;
  volatile bool _wifiConnected;
  volatile bool _wsConnected;
  volatile bool _visible;

  char _hostname[11];
  char _wifiPassword[13];
  uint8_t _mac[6];
  char _macText[18];
  char _server[16];

  volatile float _x;
  volatile float _y;
  volatile float _theta;
  volatile uint32_t _droppedMessages;

  bool _debugEnabled;

  void* _taskHandle;
  void* _txMutex;
  uint8_t _txHead;
  uint8_t _txTail;
  uint8_t _txCount;
  TxItem _txQueue[TX_QUEUE_DEPTH];

  WiFiClient* _tcpPtr;
  WebSocketClient* _wsPtr;
};

extern VisionSystemClientR4 Enes100;
extern VisionSystemClientR4& enes100;
