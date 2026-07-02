#include "01_Config_And_State.h"

// =========================================================
// 01 - CONFIG AND GLOBAL STATE
// =========================================================

String node_id = "Node 999";

bool useMainParent = true;

// -------------------------
// Raspberry Pi / Main WiFi
// -------------------------
const char* mainSsid = "Kano Dabi";
const char* mainPassword = "eimaiorizo";

String raspberryPiHeartbeatUrl = "http://192.168.1.100/heartbeat";
String raspberryPiRequestNodeIdUrl = "http://192.168.1.100/request_node_id";

// -------------------------
// Node AP settings
// -------------------------
const char* nodePassword = "12312345";
const char* parentBusyPath = "/parent_status";

// -------------------------
// Timing
// -------------------------
unsigned long lastHeartbeat = 0;

const unsigned long heartbeatInterval = 3000;
const unsigned long connectTimeout = 15000;
const unsigned long searchRetryDelay = 3000;
const unsigned long nodeIdRetryDelay = 2000;
const unsigned long childTimeout = 15000;

// -------------------------
// Web server
// -------------------------
WebServer server(80);

// -------------------------
// Node state
// -------------------------
String parent_node = "None";
String parentHeartbeatUrl = "";

bool ownApStarted = false;
bool parentBusy = false;

float temperature = 25.0;

// -------------------------
// Child storage
// -------------------------
const int MAX_CHILDREN = 10;

ChildNode children[MAX_CHILDREN];