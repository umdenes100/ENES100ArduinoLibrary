#include "WSVisionSystemClient.hpp"

VisionSystemClient *VisionSystemClient::instance = nullptr;

VisionSystemClient::VisionSystemClient()
{
    instance = this;
    x = y = theta = -1;
    visible = false;
    connected = false;
}

void VisionSystemClient::begin(const char *tName, int mType, int aId, int roomNum)
{
    teamName = tName;
    missionType = mType;
    markerId = aId;
    roomNumber = roomNum;

    // Connect to VS WiFi
    char ssid[32];
    sprintf(ssid, "VisionSystem%d-2.4", roomNumber);
    const char *password = "@R6u!n01";  

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
        delay(10);

    // WebSocket callbacks
    ws.onEvent(onWebSocketEvent);
    ws.begin("192.168.1.2", 7755, "/");
    ws.setReconnectInterval(2000);
}

void VisionSystemClient::sendPacket(JsonDocument &doc)
{
    String json;
    serializeJson(doc, json);
    ws.sendTXT(json);
}

void VisionSystemClient::onWebSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    if (!instance)
        return;

    switch (type)
    {
    case WStype_CONNECTED:
    {
        instance->connected = true;

        // Send begin doc
        StaticJsonDocument<128> doc;
        doc["op"] = "begin";
        doc["teamName"] = instance->teamName;
        doc["aruco"] = instance->markerId;
        doc["teamType"] = instance->missionType;
        instance->sendPacket(doc);
        break;
    }

    case WStype_DISCONNECTED:
        instance->connected = false;
        break;

    case WStype_TEXT:
    {
        StaticJsonDocument<256> doc;
        DeserializationError err = deserializeJson(doc, payload, length);
        if (err)
            return;

        if (doc["op"] == "aruco")
        {
            instance->visible = doc["aruco"]["visible"];
            instance->x = doc["aruco"]["x"];
            instance->y = doc["aruco"]["y"];
            instance->theta = doc["aruco"]["theta"];
        }
        break;
    }
    }
}

bool VisionSystemClient::isConnected()
{
    ws.loop();
    return connected;
}

bool VisionSystemClient::isVisible()
{
    ws.loop();
    return visible;
}

float VisionSystemClient::getX()
{
    ws.loop();
    return x;
}
float VisionSystemClient::getY()
{
    ws.loop();
    return y;
}
float VisionSystemClient::getTheta()
{
    ws.loop();
    return theta;
}

void VisionSystemClient::mission(const char *call, int msg)
{
    StaticJsonDocument<128> doc;
    doc["op"] = "mission";
    doc["teamName"] = teamName;
    doc["type"] = call;
    doc["message"] = msg;
    sendPacket(doc);
}

void VisionSystemClient::mission(const char *call, float msg)
{
    StaticJsonDocument<128> doc;
    doc["op"] = "mission";
    doc["teamName"] = teamName;
    doc["type"] = call;
    doc["message"] = msg;
    sendPacket(doc);
}

void VisionSystemClient::mission(const char *call, const char *msg)
{
    StaticJsonDocument<128> doc;
    doc["op"] = "mission";
    doc["teamName"] = teamName;
    doc["type"] = call;
    doc["message"] = msg;
    sendPacket(doc);
}

void VisionSystemClient::print(const char *msg)
{
    StaticJsonDocument<128> doc;
    doc["op"] = "print";
    doc["teamName"] = teamName;
    doc["message"] = msg;
    sendPacket(doc);
}
