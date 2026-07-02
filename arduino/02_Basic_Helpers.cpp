#include "01_Config_And_State.h"

// =========================================================
// 02 - BASIC HELPERS
// Small utility functions used by many other sections.
// =========================================================

int getNodeNumber(String name) {
  if (!name.startsWith("Node ")) {
    return 999;
  }

  String numberPart = name.substring(5);
  int number = numberPart.toInt();

  if (number <= 0) {
    number = 999;
  }

  return number;
}

bool isNodeSSID(String ssid) {
  if (!ssid.startsWith("Node ")) {
    return false;
  }

  String numberPart = ssid.substring(5);

  if (numberPart.length() == 0) {
    return false;
  }

  for (int i = 0; i < numberPart.length(); i++) {
    if (!isDigit(numberPart[i])) {
      return false;
    }
  }

  return true;
}

String getJsonStringValue(String json, String key) {
  String searchKey = "\"" + key + "\":\"";
  int startIndex = json.indexOf(searchKey);

  if (startIndex == -1) {
    return "";
  }

  startIndex += searchKey.length();
  int endIndex = json.indexOf("\"", startIndex);

  if (endIndex == -1) {
    return "";
  }

  return json.substring(startIndex, endIndex);
}

String removeSpacesAroundJson(String text) {
  text.trim();
  return text;
}
