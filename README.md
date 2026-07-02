# Raspberry Pi ESP32 Sensor Tree Project

A wireless IoT sensor network using **ESP32 nodes** and a **Raspberry Pi Flask server**.

The ESP32 boards work as sensor nodes. Each node sends its data to the Raspberry Pi directly or through another ESP32 node. The Raspberry Pi receives the data and displays the full network as a live sensor tree on a web dashboard.

---

## Project Description

This project creates a parent-child wireless sensor network.

The Raspberry Pi works as the **root server**.

Each ESP32 works as a **sensor node**.

A node can connect to:

- the Raspberry Pi server
- another ESP32 node

This allows the system to create a tree structure like this:

```text
Raspberry Pi
├── Node 1
│   ├── Node 2
│   └── Node 3
└── Node 4
```

Each node sends:

- node ID
- parent node
- temperature
- RSSI signal strength
- child nodes
- last seen time

The Raspberry Pi dashboard shows the active nodes and the full tree structure.

---

## Repository Structure

```text
Raspberry-Pi-ESP32-Sensor-Tree-Project/
├── arduino/
│   └── esp_node/
│       ├── esp_node.ino
│       ├── Config_And_State.h
│       └── other Arduino source files
│
├── server/
│   ├── server.py
│   ├── templates/
│   │   ├── index.html
│   │   └── nodes.html
│   └── static/
│       ├── style.css
│       └── nodes.js
│
└── README.md
```

---

## Main Features

- ESP32 wireless sensor nodes
- Raspberry Pi central server
- Flask web server
- Automatic node ID assignment
- Parent-child node topology
- Node-to-node communication
- Child data forwarding through parent nodes
- Live heartbeat system
- Automatic offline node removal
- RSSI signal monitoring
- Temperature data reporting
- Active nodes table
- Live tree view on the web dashboard
- Separated Arduino code and server code

---

## How the System Works

The Raspberry Pi runs a Flask server.

Each ESP32 starts as a node and searches for a parent connection.

A node can send data directly to the Raspberry Pi or to another ESP32 parent node.

When a child node sends its data to a parent, the parent stores the child data. Then the parent sends both its own data and the child data to the Raspberry Pi.

The Raspberry Pi receives all heartbeats and builds the full sensor tree.

---

## System Flow

```text
ESP32 Node
   |
   | sends heartbeat
   v
Parent ESP32 or Raspberry Pi
   |
   | forwards data
   v
Raspberry Pi Flask Server
   |
   | updates dashboard
   v
Web Browser
```

---

## Hardware Used

- Raspberry Pi
- ESP32 development boards
- WiFi network
- Temperature sensor or simulated temperature value
- Computer with Arduino IDE

---

## Software Used

- Arduino IDE
- ESP32 Arduino Core
- Python 3
- Flask
- HTML
- CSS
- JavaScript
- Git
- GitHub

---

## Arduino Code

The Arduino code is located in:

```text
arduino/esp_node/
```

The main Arduino file is:

```text
esp_node.ino
```

The Arduino code is split into different files so the project is easier to read and maintain.

---

## What the Arduino Code Does

The ESP32 code is responsible for:

- starting the ESP32 node
- creating the node access point
- searching for a parent node
- connecting to the Raspberry Pi or another ESP32
- requesting a unique node ID
- sending heartbeat data
- receiving child heartbeat data
- saving child node data
- forwarding child data to the Raspberry Pi
- sending temperature data
- sending RSSI signal strength
- keeping the node active in the network

---

## Node ID System

Each ESP32 starts with a temporary node ID.

The default temporary node ID is:

```text
Node 999
```

After the ESP32 connects to a parent or to the Raspberry Pi, it asks for a unique node ID.

The Raspberry Pi or parent system assigns a valid node name starting from:

```text
Node 1
```

This prevents two ESP32 boards from using the same node name.

---

## Parent-Child Logic

A node can have children.

A child node sends its data to its parent.

The parent node then forwards the child data together with its own heartbeat.

Example:

```text
Node 2 sends data to Node 1
Node 1 sends Node 1 data and Node 2 data to Raspberry Pi
```

This makes it possible for nodes that are far from the Raspberry Pi to still be part of the network.

---

## RSSI Signal Strength

RSSI shows the WiFi signal strength.

RSSI is measured in dBm.

The closer the value is to 0, the better the signal.

Example:

```text
-30 dBm = very strong signal
-50 dBm = good signal
-70 dBm = weak signal
-90 dBm = very poor signal
```

So:

```text
-40 dBm is better than -70 dBm
```

The project uses RSSI to help understand the signal quality of each node.

---

## Raspberry Pi Server

The Raspberry Pi server code is located in:

```text
server/
```

The main server file is:

```text
server.py
```

The server is made with Flask.

---

## What the Server Does

The Raspberry Pi server is responsible for:

- receiving heartbeat data from ESP32 nodes
- storing active node information
- assigning unique node IDs
- removing offline nodes
- building the tree structure
- serving the web dashboard
- providing API routes for the frontend
- displaying active nodes in a table
- displaying the parent-child sensor tree

---

## Web Dashboard

The web dashboard shows the current state of the sensor network.

It displays:

- active node count
- node ID
- parent node
- temperature
- RSSI
- children
- last seen time
- tree structure

Example table:

```text
Node ID     Parent          Temperature     RSSI
Node 1      Raspberry Pi    25.0 °C         -45 dBm
Node 2      Node 1          24.8 °C         -60 dBm
```

---

## API Routes

The Flask server uses routes for communication between ESP32 nodes and the dashboard.

Common routes include:

```text
/
```

Main dashboard page.

```text
/nodes
```

Active nodes page.

```text
/heartbeat
```

Receives heartbeat data from ESP32 nodes.

```text
/api/nodes
```

Returns the current active nodes as JSON.

```text
/api/tree
```

Returns the current sensor tree as JSON.

```text
/request_node_id
```

Used by ESP32 nodes to request a unique node ID.

---

## Running the Raspberry Pi Server

Go to the server folder:

```bash
cd ~/sensor_tree_server
```

Run the server:

```bash
sudo python3 server.py
```

The server should start on the Raspberry Pi IP address.

Example:

```text
Running on http://192.168.1.100:80
```

Open this address in a browser:

```text
http://192.168.1.100
```

---

## Uploading the Arduino Code

Open Arduino IDE.

Open this file:

```text
arduino/esp_node/esp_node.ino
```

Select your ESP32 board.

Select the correct USB port.

Upload the code to each ESP32 board.

Each ESP32 uses the same universal code.

---

## GitHub Repository

Project repository:

```text
https://github.com/stamatis-mavitzis/Raspberry-Pi-ESP32-Sensor-Tree-Project
```

---

## Git Commands

To upload changes to GitHub:

```bash
cd /home/smavitzis/Downloads/esp32-rpi-sensor-network

git add .
git commit -m "Update project files"
git push
```

To check the current Git status:

```bash
git status
```

To see the connected GitHub repository:

```bash
git remote -v
```

---

## Example Network

```text
Raspberry Pi
├── Node 1
│   ├── Node 2
│   │   └── Node 5
│   └── Node 3
└── Node 4
```

This means:

```text
Node 1 is connected to the Raspberry Pi
Node 2 is connected to Node 1
Node 5 is connected to Node 2
Node 3 is connected to Node 1
Node 4 is connected to the Raspberry Pi
```

---

## Project Goal

The goal of this project is to build a simple wireless sensor tree network using ESP32 boards and a Raspberry Pi server.

This project demonstrates:

- embedded systems
- IoT networking
- ESP32 WiFi communication
- parent-child network topology
- Flask backend development
- live web dashboard updates
- GitHub project organization

---

## Future Improvements

Possible improvements:

- add real temperature sensors
- add humidity sensors
- add battery voltage monitoring
- save node data in a database
- add graphs for temperature and RSSI
- improve automatic parent selection
- add node location names
- add MQTT support
- add login protection
- improve dashboard design
- add automatic reconnect logic
- add better error handling
- add signal-based routing

---

## Author

Created by **Stamatis Mavitzis**.
