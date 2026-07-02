#include "01_Config_And_State.h"

// =========================================================
// 05 - PARENT BUSY CHECK
// Child asks parent if it is free before sending heartbeat.
// =========================================================

String getParentStatusUrl() {
  IPAddress gatewayIp = WiFi.gatewayIP();
  return "http://" + gatewayIp.toString() + String(parentBusyPath);
}

bool isParentBusy() {
  if (useMainParent) {
    // Raspberry Pi is the final receiver. Do not ask it for /parent_status.
    return false;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Treating parent as busy.");
    return true;
  }

  HTTPClient http;
  String statusUrl = getParentStatusUrl();

  Serial.print("Asking parent if busy: ");
  Serial.println(statusUrl);

  http.begin(statusUrl);
  int responseCode = http.GET();

  Serial.print("Parent busy check HTTP response: ");
  Serial.println(responseCode);

  if (responseCode == 200) {
    String response = http.getString();
    response.trim();
    Serial.println(response);

    http.end();

    if (response.indexOf("\"busy\":true") >= 0) {
      Serial.println("Parent is busy. Child will wait.");
      return true;
    }

    Serial.println("Parent is free. Child can send.");
    return false;
  }

  http.end();

  Serial.println("Could not check parent status. Child will wait to avoid collision.");
  return true;
}
