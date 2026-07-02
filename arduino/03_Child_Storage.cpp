#include "01_Config_And_State.h"

// =========================================================
// 03 - CHILD STORAGE
// Saves child JSON data inside the parent node.
// =========================================================

void removeOldChildren() {
  unsigned long now = millis();

  for (int i = 0; i < MAX_CHILDREN; i++) {
    if (children[i].nodeId != "") {
      if (now - children[i].lastSeen > childTimeout) {
        Serial.print("Removing old child: ");
        Serial.println(children[i].nodeId);

        children[i].nodeId = "";
        children[i].rawJson = "";
        children[i].lastSeen = 0;
      }
    }
  }
}

void saveChildJson(String childId, String rawJson) {
  if (childId == "") {
    return;
  }

  rawJson = removeSpacesAroundJson(rawJson);

  for (int i = 0; i < MAX_CHILDREN; i++) {
    if (children[i].nodeId == childId) {
      children[i].rawJson = rawJson;
      children[i].lastSeen = millis();

      Serial.print("Updated child data: ");
      Serial.println(childId);
      return;
    }
  }

  for (int i = 0; i < MAX_CHILDREN; i++) {
    if (children[i].nodeId == "") {
      children[i].nodeId = childId;
      children[i].rawJson = rawJson;
      children[i].lastSeen = millis();

      Serial.print("New child saved: ");
      Serial.println(childId);
      return;
    }
  }

  Serial.println("Child table full. Could not save child.");
}
