<div align="center">

# ESP32 Raspberry Pi Sensor Tree Project

### Wireless ESP32 Sensor Network with Raspberry Pi Dashboard

![C++](https://img.shields.io/badge/C++-Arduino-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Python](https://img.shields.io/badge/Python-Flask-3776AB?style=for-the-badge&logo=python&logoColor=white)
![HTML](https://img.shields.io/badge/HTML-Frontend-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![CSS](https://img.shields.io/badge/CSS-Styling-1572B6?style=for-the-badge&logo=css3&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-Dashboard-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)
![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-Server-A22846?style=for-the-badge&logo=raspberrypi&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-Wireless%20Nodes-black?style=for-the-badge)
![License](https://img.shields.io/badge/License-Educational-green?style=for-the-badge)

</div>

---

## Overview

The **ESP32 Raspberry Pi Sensor Tree Project** is a wireless IoT sensor network where multiple ESP32 devices connect together using a parent-child tree structure and send sensor data to a Raspberry Pi server.

The Raspberry Pi runs a **Python Flask web dashboard** that displays active nodes, parent nodes, signal strength, temperature values, children, and the live network tree.

This project combines **embedded systems**, **wireless networking**, **web development**, **Linux server management**, and **IoT dashboard design**.

---

## Project Description

This project demonstrates a wireless sensor-tree network using ESP32 nodes and a Raspberry Pi Flask server.

Each ESP32 can act as a node in the network. A node can connect directly to the Raspberry Pi WiFi network or connect through another ESP32 node. This creates a flexible tree-like topology where data can travel from child nodes to parent nodes and finally to the Raspberry Pi dashboard.

The system is designed so that ESP32 nodes can form a simple wireless hierarchy:

```text
Child Node → Parent Node → Raspberry Pi Server → Web Dashboard
```

---

## ️ Languages and Technologies Used

| Language / Technology | Usage |
|---|---|
| **C++ / Arduino** | ESP32 firmware, WiFi connection logic, HTTP communication, node server, heartbeat system |
| **Python** | Flask backend server running on the Raspberry Pi |
| **HTML** | Web dashboard page structure |
| **CSS** | Dashboard styling, layout, buttons, tables, and visual design |
| **JavaScript** | Live data fetching, table updates, and tree rendering |
| **JSON** | Data format used between ESP32 nodes and the Raspberry Pi server |
| **Bash / Linux Commands** | Running, copying, deploying, and managing the project on Raspberry Pi |
| **Git / GitHub** | Version control and project hosting |

---

## Main Features

### ESP32 Node Features

- Wireless ESP32 node system
- Parent-child node communication
- Automatic node registration
- Unique node naming
- RSSI signal strength monitoring
- Temperature value support
- Heartbeat system
- Child data forwarding
- Local ESP32 access point support
- Parent status check before sending child data
- Modular Arduino/C++ file structure

### ️ Raspberry Pi Server Features

- Python Flask backend server
- Receives heartbeat data from nodes
- Stores active node information
- Removes offline nodes automatically
- Provides JSON API routes
- Serves the dashboard pages
- Displays the live sensor network state

### Dashboard Features

- Home page with navigation
- Active nodes table
- Parent node display
- RSSI display
- Temperature display
- Children display
- Last seen time
- Tree view of the network
- Live updates without manually refreshing the page

---

## ️ System Architecture

```text
┌─────────────────────┐
│   ESP32 Child Node  │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│  ESP32 Parent Node  │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Raspberry Pi Server │
│   Python + Flask    │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│    Web Dashboard    │
│ HTML / CSS / JS     │
└─────────────────────┘
```

The Raspberry Pi acts as the main server. ESP32 nodes send heartbeat data either directly to the Raspberry Pi or through another ESP32 parent node.

---

## How It Works

1. The Raspberry Pi starts the Flask server.
2. The ESP32 boots and starts its setup logic.
3. The ESP32 searches for a parent connection.
4. If the Raspberry Pi network is available and strong enough, the ESP32 can connect directly to it.
5. If the Raspberry Pi network is weak or unavailable, the ESP32 searches for another ESP32 node.
6. The node connects to the best available parent.
7. The node requests or receives a unique node ID.
8. The node starts sending heartbeat data.
9. Parent nodes receive data from child nodes.
10. Parent nodes forward child information upward.
11. The Raspberry Pi stores the latest node data.
12. The dashboard displays the network as a table and as a tree.

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

The code is split into multiple files so the project is cleaner, easier to debug, and easier to expand.

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

### Setup and Loop Structure

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

## Important Arduino Files

| File | Purpose |
|---|---|
| `esp_node.ino` | Main Arduino entry file with `setup()` and `loop()` |
| `Config_And_State.h` | Global variables, configuration, includes, and function declarations |
| `01_Json_Helpers.cpp` | Helper functions for JSON parsing and text cleanup |
| `02_Child_Storage.cpp` | Stores and updates child node data |
| `03_Tree_Builder.cpp` | Builds JSON tree data |
| `04_Parent_Status.cpp` | Checks if the parent node is busy |
| `05_Node_Id.cpp` | Handles unique node ID requests |
| `06_Node_Server.cpp` | Runs the ESP32 web server endpoints |
| `07_WiFi_Connection.cpp` | Handles WiFi scanning and parent connection |
| `08_Heartbeat.cpp` | Sends heartbeat data and child data |
| `09_Main_Node_Logic.cpp` | Main node connection and runtime logic |
| `10_Node_Setup_And_Loop.cpp` | Node setup and loop functions |

---

## ️ Raspberry Pi Server

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

## Example Network Flow

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

## How to Run the Raspberry Pi Server

Go to the server folder:

```bash
cd ~/sensor_tree_server
```

Run the Flask server:

```bash
python3 server.py
```

If the server uses port `80`, run it with sudo:

```bash
sudo python3 server.py
```

Then open the dashboard in a browser:

```text
http://192.168.1.100
```

---

## GitHub Upload Commands

From the main project folder:

```bash
cd ~/Downloads/esp32-rpi-sensor-network
```

Add the files:

```bash
git add arduino server README.md
```

Commit the changes:

```bash
git commit -m "Update ESP32 Raspberry Pi sensor tree project"
```

Push to GitHub:

```bash
git push origin main
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
- Add dashboard settings page
- Add signal quality color indicators
- Add node uptime
- Add automatic Raspberry Pi startup service
- Add historical logs for node connections

---

## Educational Purpose

This project was created for learning and demonstration purposes.

It combines:

- Embedded systems
- ESP32 WiFi networking
- Raspberry Pi server deployment
- Flask backend development
- Frontend dashboard design
- REST API communication
- JSON data handling
- Linux command-line usage
- IoT system architecture
- Parent-child network topology

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
- Git and GitHub usage
- IoT system architecture
- Parent-child network topology
- Modular code organization

---

## ‍ Credits

Developed as an educational IoT project using:

- ESP32 microcontrollers
- Raspberry Pi
- Arduino framework
- Python Flask
- HTML, CSS, and JavaScript

---

## License

This project is for educational use.

<div align="center">

---

### ESP32 + Raspberry Pi + Flask + IoT Dashboard

</div>
