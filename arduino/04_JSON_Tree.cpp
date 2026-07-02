#include "01_Config_And_State.h"

// =========================================================
// 04 - JSON TREE
// Builds the JSON sent upward to the parent/Raspberry Pi.
// =========================================================

String makeTreeJson() {
  removeOldChildren();

  String json = "{";
  json += "\"node_id\":\"" + node_id + "\",";
  json += "\"parent_node\":\"" + parent_node + "\",";
  json += "\"temperature\":" + String(temperature, 2) + ",";
  json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
  json += "\"children\":[";

  bool firstChild = true;

  for (int i = 0; i < MAX_CHILDREN; i++) {
    if (children[i].nodeId != "" && children[i].rawJson != "") {
      if (!firstChild) {
        json += ",";
      }

      // Inserts the full child JSON object, not only "Node 2".
      json += children[i].rawJson;
      firstChild = false;
    }
  }

  json += "]";
  json += "}";

  return json;
}

String makeOwnJsonOnly() {
  // Debug only. Normal heartbeat uses makeTreeJson().
  String json = "{";
  json += "\"node_id\":\"" + node_id + "\",";
  json += "\"parent_node\":\"" + parent_node + "\",";
  json += "\"temperature\":" + String(temperature, 2) + ",";
  json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
  json += "\"children\":[]";
  json += "}";

  return json;
}
