#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

class VisionSystemClient
{
public:
    VisionSystemClient();

    void begin(const char *teamName, int missionType, int markerId, int roomNumber);
    void mission(const char *missionCall, const char *msg);
    void mission(const char *missionCall, int msg);
    void mission(const char *missionCall, float msg);

    bool isConnected();
    bool isVisible();

    float getX();
    float getY();
    float getTheta();

    void print(const char *message);

private:
    void sendPacket(JsonDocument &doc);
    static void onWebSocketEvent(WStype_t type, uint8_t *payload, size_t length);

    static VisionSystemClient *instance;
    WebSocketsClient ws;

    String teamName;
    int missionType;
    int markerId;
    int roomNumber;

    volatile float x, y, theta;
    volatile bool visible;
    volatile bool connected;
};
