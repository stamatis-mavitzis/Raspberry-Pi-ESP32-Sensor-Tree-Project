#include "01_Config_And_State.h"

// =========================================================
// 07 - ESP WEB SERVER HANDLERS
// Endpoints used by children and for browser debugging.
// =========================================================

void handleChildHeartbeat() {
  if (parentBusy) {
    server.send(409, "application/json", "{\"status\":\"busy\",\"busy\":true}");
    return;
  }

  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"no body received\"}");
    return;
  }

  String body = server.arg("plain");
  body.trim();

  Serial.println();
  Serial.println("================================");
  Serial.println("Received heartbeat from child");
  Serial.println("================================");
  Serial.println(body);

  String childId = getJsonStringValue(body, "node_id");

  if (childId == "") {
    server.send(400, "application/json", "{\"error\":\"missing node_id\"}");
    return;
  }

  saveChildJson(childId, body);

  Serial.print("Child ");
  Serial.print(childId);
  Serial.println(" saved inside parent.");

  Serial.println("Parent tree is now:");
  Serial.println(makeTreeJson());

  server.send(200, "application/json", "{\"status\":\"child saved in parent\"}");
}

void handleNodeIdRequest() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"no body received\"}");
    return;
  }

  String body = server.arg("plain");

  Serial.println();
  Serial.println("Child requested unique node id:");
  Serial.println(body);

  String forwardUrl = "";

  if (useMainParent) {
    forwardUrl = raspberryPiRequestNodeIdUrl;
  } else {
    IPAddress gatewayIp = WiFi.gatewayIP();
    forwardUrl = "http://" + gatewayIp.toString() + "/request_node_id";
  }

  Serial.print("Forwarding node id request to: ");
  Serial.println(forwardUrl);

  if (WiFi.status() != WL_CONNECTED) {
    server.send(500, "application/json", "{\"error\":\"wifi not connected\"}");
    return;
  }

  HTTPClient http;
  http.begin(forwardUrl);
  http.addHeader("Content-Type", "application/json");

  int responseCode = http.POST(body);

  Serial.print("Forward response code: ");
  Serial.println(responseCode);

  if (responseCode > 0) {
    String response = http.getString();

    Serial.println("Forwarded node id response:");
    Serial.println(response);

    server.send(responseCode, "application/json", response);
  } else {
    Serial.print("Forward HTTP error: ");
    Serial.println(http.errorToString(responseCode));

    server.send(500, "application/json", "{\"error\":\"failed to forward node id request\"}");
  }

  http.end();
}

void handleParentStatus() {
  String json = "{";
  json += "\"node_id\":\"" + node_id + "\",";
  json += "\"busy\":";
  json += (parentBusy ? "true" : "false");
  json += "}";

  server.send(200, "application/json", json);
}

void startNodeServer() {
  server.on("/child_heartbeat", HTTP_POST, handleChildHeartbeat);
  server.on("/request_node_id", HTTP_POST, handleNodeIdRequest);
  server.on(parentBusyPath, HTTP_GET, handleParentStatus);

  server.on("/", HTTP_GET, []() {
    String html = "";
    html += "<h1>" + node_id + "</h1>";
    html += "<p>Parent: " + parent_node + "</p>";
    html += "<p>Tree JSON:</p>";
    html += "<pre>";
    html += makeTreeJson();
    html += "</pre>";

    server.send(200, "text/html", html);
  });

  server.begin();

  Serial.println("Node web server started on port 80.");
  Serial.println("Children can send heartbeat to:");
  Serial.println("http://PARENT_GATEWAY_IP/child_heartbeat");
}
