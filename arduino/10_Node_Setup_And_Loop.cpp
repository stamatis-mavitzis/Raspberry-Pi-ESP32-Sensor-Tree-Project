#include "01_Config_And_State.h"

// =========================================================
// 10 - NODE SETUP AND LOOP
// High-level startup and repeated loop logic.
// =========================================================

void nodeSetup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32 UNIVERSAL NODE");
  Serial.println("================================");
  Serial.print("Temporary starting node_id = ");
  Serial.println(node_id);
  Serial.print("useMainParent = ");
  Serial.println(useMainParent ? "true" : "false");

  WiFi.mode(WIFI_AP_STA);

  if (useMainParent) {
    Serial.println();
    Serial.println("Root / Raspberry Pi connection mode");
    Serial.println("-----------------------------------");
    Serial.println("This ESP32 connects directly to Raspberry Pi/main WiFi.");
    Serial.println("It starts as Node 999, then asks Raspberry Pi for Node 1+.");

    bool connected = false;

    while (!connected) {
      connected = connectToMainWiFi();

      if (!connected) {
        Serial.print("Retrying main WiFi in ");
        Serial.print(searchRetryDelay / 1000);
        Serial.println(" seconds...");
        delay(searchRetryDelay);
      }
    }

    waitUntilUniqueNodeId(raspberryPiRequestNodeIdUrl);

    startOwnAP();
    startNodeServer();

    Serial.print("After starting AP, STA status = ");
    Serial.println(WiFi.status());

    Serial.print("Still connected to Raspberry Pi WiFi? ");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("YES");
      Serial.print("STA IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("Gateway IP: ");
      Serial.println(WiFi.gatewayIP());
    } else {
      Serial.println("NO");
    }

  } else {
    Serial.println();
    Serial.println("Child / Node parent connection mode");
    Serial.println("-----------------------------------");
    Serial.println("This ESP32 searches for a Node parent.");
    Serial.println("It starts as Node 999, connects to root/leaf, then asks for Node 1+.");

    bool connectedToParent = false;

    while (!connectedToParent) {
      connectedToParent = connectToParentNode();

      if (!connectedToParent) {
        Serial.print("Retrying in ");
        Serial.print(searchRetryDelay / 1000);
        Serial.println(" seconds...");
        delay(searchRetryDelay);
      }
    }

    startOwnAP();
    startNodeServer();

    Serial.print("After starting AP, STA status = ");
    Serial.println(WiFi.status());

    Serial.print("Still connected to parent? ");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("YES");
      Serial.print("STA IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("Gateway / parent IP: ");
      Serial.println(WiFi.gatewayIP());
      Serial.print("Parent heartbeat URL: ");
      Serial.println(parentHeartbeatUrl);
    } else {
      Serial.println("NO");
    }
  }
}

void nodeLoop() {
  server.handleClient();

  unsigned long now = millis();

  if (now - lastHeartbeat >= heartbeatInterval) {
    lastHeartbeat = now;

    if (useMainParent) {
      sendHeartbeatAndChildren(raspberryPiHeartbeatUrl);
    } else {
      if (parentHeartbeatUrl != "") {
        sendChildHeartbeatWhenParentFree(parentHeartbeatUrl);
      } else {
        Serial.println("No parentHeartbeatUrl set.");
      }
    }
  }
}
