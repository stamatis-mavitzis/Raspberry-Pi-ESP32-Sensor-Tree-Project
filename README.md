# ESP32 Raspberry Pi Sensor Tree Project

![C++](https://img.shields.io/badge/C++-Arduino-blue)
![Python](https://img.shields.io/badge/Python-Flask-yellow)
![HTML](https://img.shields.io/badge/HTML-Frontend-orange)
![CSS](https://img.shields.io/badge/CSS-Styling-blueviolet)
![JavaScript](https://img.shields.io/badge/JavaScript-Dashboard-brightgreen)
![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-Server-red)
![ESP32](https://img.shields.io/badge/ESP32-Wireless%20Nodes-black)
![License](https://img.shields.io/badge/license-Educational-green)

---

## Table of Contents

- [Overview](#overview)
- [Project Description](#project-description)
- [Languages Used](#languages-used)
- [Features](#features)
- [System Architecture](#system-architecture)
- [How It Works](#how-it-works)
- [Project Structure](#project-structure)
- [Arduino Node Logic](#arduino-node-logic)
- [Raspberry Pi Server](#raspberry-pi-server)
- [Dashboard Pages](#dashboard-pages)
- [Network Flow](#network-flow)
- [Future Improvements](#future-improvements)
- [Credits](#credits)

---

## Overview

The **ESP32 Raspberry Pi Sensor Tree Project** is a wireless IoT sensor network where multiple ESP32 devices connect together in a parent-child tree structure and send sensor data to a Raspberry Pi server.

The Raspberry Pi runs a Flask web dashboard that displays the active nodes, their parents, signal strength, temperature, children, and live tree structure.

---

## Project Description

This project demonstrates a wireless sensor-tree network using ESP32 nodes and a Raspberry Pi Flask server.

Each ESP32 can act as a node in the network. A node can connect directly to the Raspberry Pi WiFi network or connect through another ESP32 node. This creates a flexible tree-like topology where data can travel from child nodes to parent nodes and finally to the Raspberry Pi dashboard.

---

## Languages Used

| Language | Usage |
|---|---|
| **C++ / Arduino** | ESP32 node firmware, WiFi logic, HTTP communication, sensor handling |
| **Python** | Flask backend server running on the Raspberry Pi |
| **HTML** | Web dashboard pages |
| **CSS** | Dashboard styling and layout |
| **JavaScript** | Live table updates, tree rendering, API fetching |
| **JSON** | Data exchange between ESP32 nodes and the Raspberry Pi server |
| **Bash / Linux Commands** | Running, copying, deploying, and managing the project on Raspberry Pi |

---

## Features

### Core Features

- ESP32 wireless node system
- Raspberry Pi Flask server
- Live web dashboard
- Active node table
- Tree view of parent-child relationships
- Automatic node registration
- Unique node naming
- Parent and child communication
- RSSI signal strength monitoring
- Temperature value support
- Node timeout and automatic removal
- JSON-based communication
- Simple modular project structure

### Dashboard Features

- Shows active nodes
- Shows each node parent
- Shows RSSI value
- Shows temperature value
- Shows child nodes
- Shows last seen time
- Displays the full sensor tree
- Updates data without manually refreshing the page

---

## System Architecture

The project follows a simple IoT tree architecture:

```text
ESP32 Child Node
        ↓
ESP32 Parent Node
        ↓
Raspberry Pi Flask Server
        ↓
Web Dashboard
```

The Raspberry Pi acts as the main server. ESP32 nodes send heartbeat data to the server or to another ESP32 parent node.

---

## How It Works

1. The Raspberry Pi runs the Flask server.
2. The ESP32 starts and searches for a parent.
3. If the Raspberry Pi network is available, the ESP32 can connect to it.
4. If the Raspberry Pi network is weak or unavailable, the ESP32 searches for another ESP32 node.
5. The node connects to the strongest available parent.
6. The node requests or receives a unique node ID.
7. The node sends heartbeat data.
8. Parent nodes forward child information.
9. The Raspberry Pi stores the latest node data.
10. The dashboard displays the network as a table and as a tree.

---

## Project Structure

```text
esp32-rpi-sensor-network/
│
├── arduino/
│   └── esp_node/
│       ├── esp_node.ino
│       ├── Config_And_State.h
│       ├── 01_Json_Helpers.cpp
│       ├── 02_Child_Storage.cpp
│       ├── 03_Tree_Builder.cpp
│       ├── 04_Parent_Status.cpp
│       ├── 05_Node_Id.cpp
│       ├── 06_Node_Server.cpp
│       ├── 07_WiFi_Connection.cpp
│       ├── 08_Heartbeat.cpp
│       ├── 09_Main_Node_Logic.cpp
│       └── 10_Node_Setup_And_Loop.cpp
│
├── server/
│   ├── server.py
│   ├── templates/
│   │   ├── index.html
│   │   ├── nodes.html
│   │   └── tree.html
│   │
│   └── static/
│       ├── style.css
│       ├── nodes.js
│       └── tree.js
│
└── README.md
```

---

## Arduino Node Logic

The ESP32 firmware is written in **C++ using the Arduino framework**.

The code is split into multiple files to keep the project clean and easy to understand.

### Main Node Responsibilities

- Start serial communication
- Start its own WiFi access point
- Search for a parent node
- Connect to the Raspberry Pi or another ESP32
- Ask for a unique node ID
- Start a small web server
- Send heartbeat data
- Receive heartbeat data from child nodes
- Forward child information to the parent
- Keep track of connected children

### Setup and Loop

The main `.ino` file stays simple:

```cpp
void setup() {
    nodeSetup();
}

void loop() {
    nodeLoop();
}
```

The actual setup and loop logic is placed inside:

```text
10_Node_Setup_And_Loop.cpp
```

This makes the project easier to expand later with sensors, extra logic, or new features.

---

## Raspberry Pi Server

The Raspberry Pi runs a **Python Flask server**.

The server receives data from ESP32 nodes and stores the latest state of the network.

### Server Responsibilities

- Receive heartbeat requests
- Store active nodes
- Remove offline nodes
- Provide API data for the dashboard
- Serve the table page
- Serve the tree page
- Manage node data in JSON format

Example server flow:

```text
ESP32 Node → HTTP POST /heartbeat → Flask Server → Dashboard API → Browser
```

---

## Dashboard Pages

The web dashboard is built using:

- **HTML**
- **CSS**
- **JavaScript**
- **Flask templates**

### Home Page

```text
/
```

The home page provides navigation buttons to the node table and tree view.

### Nodes Table

```text
/nodes
```

Displays all active nodes in a table.

### Tree View

```text
/tree
```

Displays the parent-child structure of the ESP32 sensor network.

### API Routes

```text
/api/nodes
/api/tree
```

These routes return live JSON data for the frontend JavaScript code.

---

## Network Flow

```text
Raspberry Pi
│
├── Node 1
│   ├── Node 2
│   └── Node 3
│
└── Node 4
    └── Node 5
```

Each node knows its parent and can also store information about its children.

This allows the system to build a live tree of the whole wireless sensor network.

---

## Example Node Data

```json
{
  "node_id": "Node 2",
  "parent_node": "Node 1",
  "temperature": 25.0,
  "rssi": -45,
  "children": [],
  "last_seen": "2 seconds ago"
}
```

---

## RSSI Signal Strength

RSSI shows the WiFi signal strength.

The value is usually negative.

```text
-30 dBm = Very strong signal
-50 dBm = Good signal
-70 dBm = Weak signal
-90 dBm = Very bad signal
```

A value closer to `0` is better.

For example:

```text
-40 is better than -80
```

---

## Future Improvements

Possible improvements for the project:

- Add real sensors such as DHT11, DHT22, BMP280, or soil moisture sensors
- Add database storage
- Add charts for temperature history
- Add battery monitoring
- Add node rename from the dashboard
- Add automatic closest-parent selection
- Add WebSocket live updates
- Add login page for dashboard security
- Add Docker support
- Add mobile-friendly dashboard design
- Add export to CSV or JSON
- Add node location names
- Add alerts when a node goes offline

---

## Educational Purpose

This project was created for learning and demonstration purposes.

It combines embedded systems, wireless networking, web development, Linux server management, and IoT dashboard design.

---

## Skills Demonstrated

- Embedded systems programming
- ESP32 WiFi networking
- Arduino C++ development
- Raspberry Pi server deployment
- Flask backend development
- REST API communication
- JSON data handling
- Frontend dashboard development
- HTML/CSS/JavaScript
- Linux command-line usage
- IoT system architecture
- Parent-child network topology

---

## Credits

Developed as an educational IoT project using:

- ESP32 microcontrollers
- Raspberry Pi
- Arduino framework
- Python Flask
- HTML, CSS, and JavaScript

---

## License

This project is for educational use.
