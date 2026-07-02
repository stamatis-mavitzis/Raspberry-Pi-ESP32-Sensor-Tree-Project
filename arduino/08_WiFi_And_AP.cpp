#include "01_Config_And_State.h"

// =========================================================
// 08 - WIFI AND ACCESS POINT
// Connects to Raspberry Pi WiFi or to another Node parent.
// =========================================================

void startOwnAP() {
  int nodeNumber = getNodeNumber(node_id);

  if (nodeNumber == 999) {
    Serial.println("ERROR: Still Node 999. Refusing to start AP before unique name.");
    return;
  }

  // Different AP subnet for every node.
  // Node 1 -> 192.168.101.1
  // Node 2 -> 192.168.102.1
  // Node 3 -> 192.168.103.1
  IPAddress apIP(192, 168, 100 + nodeNumber, 1);
  IPAddress gateway(192, 168, 100 + nodeNumber, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(apIP, gateway, subnet);

  Serial.print("Starting own AP: ");
  Serial.println(node_id);

  bool ok = WiFi.softAP(node_id.c_str(), nodePassword);

  if (ok) {
    ownApStarted = true;

    Serial.println("Own AP started.");
    Serial.print("Own AP IP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start own AP.");
  }
}

bool connectToMainWiFi() {
  Serial.print("Connecting to main WiFi: ");
  Serial.println(mainSsid);

  WiFi.begin(mainSsid, mainPassword);

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < connectTimeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    parent_node = "Raspberry Pi";

    Serial.println("Connected to main WiFi.");
    Serial.print("ESP32 STA IP: ");
    Serial.println(WiFi.localIP());

    return true;
  }

  Serial.println("Failed to connect to main WiFi.");
  return false;
}

String searchForStrongestNodeSSID() {
  Serial.println("Searching for strongest Node x WiFi...");

  int networks = WiFi.scanNetworks();

  String strongestNode = "";
  int strongestRssi = -999;

  for (int i = 0; i < networks; i++) {
    String foundSsid = WiFi.SSID(i);
    int foundRssi = WiFi.RSSI(i);

    Serial.print("Found WiFi: ");
    Serial.print(foundSsid);
    Serial.print(" RSSI: ");
    Serial.println(foundRssi);

    if (isNodeSSID(foundSsid)) {
      if (ownApStarted && foundSsid == node_id) {
        Serial.print("Ignoring own AP: ");
        Serial.println(foundSsid);
        continue;
      }

      Serial.print("Usable Node parent found: ");
      Serial.println(foundSsid);

      if (foundRssi > strongestRssi) {
        strongestRssi = foundRssi;
        strongestNode = foundSsid;
      }
    }
  }

  WiFi.scanDelete();

  if (strongestNode != "") {
    Serial.print("Strongest parent selected: ");
    Serial.println(strongestNode);
  } else {
    Serial.println("No Node x WiFi found.");
  }

  return strongestNode;
}

bool connectToParentNode() {
  Serial.println("Searching for Node x parent...");

  String parentSsid = searchForStrongestNodeSSID();

  if (parentSsid == "") {
    Serial.println("No usable Node x parent found.");
    return false;
  }

  Serial.print("Trying to connect to parent node: ");
  Serial.println(parentSsid);

  WiFi.begin(parentSsid.c_str(), nodePassword);

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < connectTimeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    parent_node = parentSsid;

    IPAddress gatewayIp = WiFi.gatewayIP();

    // Child sends to parent gateway IP, not directly to Raspberry Pi.
    parentHeartbeatUrl = "http://" + gatewayIp.toString() + "/child_heartbeat";
    String parentRequestNodeIdUrl = "http://" + gatewayIp.toString() + "/request_node_id";

    Serial.println("Connected to parent node!");
    Serial.print("Parent SSID: ");
    Serial.println(parent_node);
    Serial.print("Child STA IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway IP / parent IP: ");
    Serial.println(gatewayIp);
    Serial.print("Parent heartbeat URL: ");
    Serial.println(parentHeartbeatUrl);
    Serial.print("Parent request node id URL: ");
    Serial.println(parentRequestNodeIdUrl);

    waitUntilUniqueNodeId(parentRequestNodeIdUrl);

    return true;
  }

  Serial.println("Failed to connect to parent node.");
  return false;
}
