#include "01_Config_And_State.h"

// =========================================================
// 09 - HEARTBEAT SENDING
// Sends this node and saved child data upward.
// =========================================================

bool postJson(String url, String json, String label) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Cannot send data.");
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
    return false;
  }

  HTTPClient http;

  Serial.println();
  Serial.println("================================");
  Serial.println(label);
  Serial.println("================================");
  Serial.print("To: ");
  Serial.println(url);
  Serial.println(json);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int responseCode = http.POST(json);

  Serial.print("HTTP response: ");
  Serial.println(responseCode);

  if (responseCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(responseCode));
  }

  http.end();

  return responseCode >= 200 && responseCode < 300;
}

void sendHeartbeatAndChildren(String url) {
  removeOldChildren();

  parentBusy = true;

  // 1) Send this node tree first.
  // The tree contains the children list, so the Raspberry Pi knows the relationship.
  postJson(url, makeTreeJson(), "Sending parent tree heartbeat first");

  // 2) Forward every saved child JSON upward as its own heartbeat too.
  // This keeps each child as a separate row on the Raspberry Pi page.
  for (int i = 0; i < MAX_CHILDREN; i++) {
    if (children[i].nodeId != "" && children[i].rawJson != "") {
      String label = "Forwarding saved child heartbeat: " + children[i].nodeId;
      postJson(url, children[i].rawJson, label);
      delay(100);
    }
  }

  parentBusy = false;
}

void sendChildHeartbeatWhenParentFree(String url) {
  if (isParentBusy()) {
    Serial.println("Skipping this heartbeat. Parent is busy.");
    return;
  }

  sendHeartbeatAndChildren(url);
}
