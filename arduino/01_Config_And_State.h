#ifndef CONFIG_AND_STATE_H
#define CONFIG_AND_STATE_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>

// =========================================================
// Function declarations
// =========================================================

void nodeSetup();
void nodeLoop();

int getNodeNumber(String name);
bool isNodeSSID(String ssid);
String getJsonStringValue(String json, String key);
String removeSpacesAroundJson(String text);

void removeOldChildren();
void saveChildJson(String childId, String rawJson);

String makeTreeJson();
String makeOwnJsonOnly();

String getParentStatusUrl();
bool isParentBusy();

bool askForUniqueNodeId(String requestUrl);
void waitUntilUniqueNodeId(String requestUrl);

void handleChildHeartbeat();
void handleNodeIdRequest();
void handleParentStatus();
void startNodeServer();

void startOwnAP();
bool connectToMainWiFi();
String searchForStrongestNodeSSID();
bool connectToParentNode();

bool postJson(String url, String json, String label);
void sendHeartbeatAndChildren(String url);
void sendChildHeartbeatWhenParentFree(String url);

// =========================================================
// Global variable declarations
// =========================================================

extern String node_id;

extern bool useMainParent;

extern const char* mainSsid;
extern const char* mainPassword;

extern String raspberryPiHeartbeatUrl;
extern String raspberryPiRequestNodeIdUrl;

extern const char* nodePassword;
extern const char* parentBusyPath;

extern unsigned long lastHeartbeat;
extern const unsigned long heartbeatInterval;

extern const unsigned long connectTimeout;
extern const unsigned long searchRetryDelay;
extern const unsigned long nodeIdRetryDelay;
extern const unsigned long childTimeout;

extern WebServer server;

extern String parent_node;
extern String parentHeartbeatUrl;

extern bool ownApStarted;
extern bool parentBusy;

extern float temperature;

extern const int MAX_CHILDREN;

struct ChildNode {
  String nodeId;
  String rawJson;
  unsigned long lastSeen;
};

extern ChildNode children[];

#endif