#include "01_Config_And_State.h"

// =========================================================
// 06 - UNIQUE NODE ID REQUEST
// Gets Node 1, Node 2, Node 3... from Raspberry Pi through parent.
// =========================================================

bool askForUniqueNodeId(String requestUrl) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Cannot request node id.");
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
    return false;
  }

  HTTPClient http;

  Serial.println();
  Serial.print("Requesting unique node id from: ");
  Serial.println(requestUrl);

  http.begin(requestUrl);
  http.addHeader("Content-Type", "application/json");

  String json = "{";
  json += "\"current_node_id\":\"" + node_id + "\",";
  json += "\"parent_node\":\"" + parent_node + "\",";
  json += "\"device_id\":\"" + WiFi.macAddress() + "\"";
  json += "}";

  int responseCode = http.POST(json);

  Serial.print("Node id request response: ");
  Serial.println(responseCode);

  if (responseCode > 0) {
    String response = http.getString();
    Serial.println(response);

    if (responseCode == 200) {
      String newNodeId = getJsonStringValue(response, "node_id");

      if (newNodeId != "") {
        node_id = newNodeId;

        Serial.print("Assigned unique node id: ");
        Serial.println(node_id);

        http.end();
        return true;
      }
    }
  } else {
    Serial.print("Node id HTTP error: ");
    Serial.println(http.errorToString(responseCode));
  }

  http.end();

  Serial.println("Failed to get unique node id.");
  return false;
}

void waitUntilUniqueNodeId(String requestUrl) {
  while (!askForUniqueNodeId(requestUrl)) {
    Serial.print("Retrying node id request in ");
    Serial.print(nodeIdRetryDelay / 1000);
    Serial.println(" seconds...");
    delay(nodeIdRetryDelay);
  }
}
