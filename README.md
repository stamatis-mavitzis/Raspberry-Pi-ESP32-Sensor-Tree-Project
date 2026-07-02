# Raspberry Pi ESP32 Sensor Tree Project

This project creates a wireless sensor tree network using ESP32 nodes and a Raspberry Pi server.

Each ESP32 node can connect either directly to the Raspberry Pi WiFi network or to another ESP32 node. The nodes send heartbeat data, temperature, RSSI signal strength, parent information, and child node data to the Raspberry Pi server.

The Raspberry Pi runs a Flask web server that displays the active nodes in a table and tree view.

---

## Project Idea

The goal of this project is to build a simple embedded IoT network where multiple ESP32 boards organize themselves into a parent-child tree.

The Raspberry Pi acts as the root server.

Example structure:

```text
Raspberry Pi
├── Node 1
│   ├── Node 2
│   └── Node 3
└── Node 4
```

Each node sends its own data and can also forward child data to its parent.

---

## Features

- ESP32 universal node code
- Raspberry Pi Flask server
- Automatic node ID assignment
- Parent-child node connection
- Node heartbeat system
- RSSI signal strength reporting
- Temperature reporting
- Web dashboard
- Active nodes table
- Tree view of connected nodes
- Automatic removal of offline nodes
- Child data forwarding from node to parent
- JSON API for node data
- Optional debug mode using `useMainParent`

---

## Hardware Used

- Raspberry Pi
- ESP32 development boards
- WiFi network
- Optional temperature sensor

---

## Software Used

### Raspberry Pi

- Python 3
- Flask
- HTML
- CSS
- JavaScript

### ESP32

- Arduino IDE
- ESP32 board package
- WiFi library
- WebServer library
- HTTPClient library
- ArduinoJson library

---

## Folder Structure

```text
sensor_tree_server/
├── server.py
├── templates/
│   ├── index.html
│   ├── nodes.html
│   └── tree.html
├── static/
│   ├── style.css
│   ├── nodes.js
│   └── tree.js
└── README.md
```

ESP32 code example:

```text
esp_node/
├── esp_node.ino
├── Config_And_State.h
├── 10_Node_Setup_And_Loop.cpp
├── 20_Json_Functions.cpp
├── 30_Server_Handlers.cpp
├── 40_WiFi_Functions.cpp
└── 50_Heartbeat_Functions.cpp
```

---

## How It Works

### 1. Raspberry Pi Server

The Raspberry Pi runs a Flask server.

The server receives heartbeat data from the ESP32 nodes through HTTP POST requests.

Each heartbeat contains information such as:

```json
{
  "node_id": "Node 1",
  "parent_node": "Raspberry Pi",
  "temperature": 25.0,
  "rssi": -43,
  "children": []
}
```

The server stores the latest information for each active node.

If a node stops sending heartbeats for a few seconds, it is removed from the active node list.

---

### 2. ESP32 Nodes

Each ESP32 starts with a temporary node name:

```text
Node 999
```

After connecting to the Raspberry Pi or to another node, it asks for a unique node ID.

Example:

```text
Node 1
Node 2
Node 3
```

Each ESP32 also starts its own WiFi access point using its node name.

Example:

```text
SSID: Node 1
```

Other nodes can then connect to it.

---

### 3. Parent Selection

Each node searches for available parent connections.

The normal logic is:

```text
If Raspberry Pi WiFi is visible and strong enough:
    connect to Raspberry Pi
else:
    search for the strongest nearby Node x WiFi
    connect to that node
```

RSSI is used to estimate signal strength.

Important:

```text
RSSI closer to 0 is better.
```

Example:

```text
-40 dBm is stronger than -80 dBm
```

---

### 4. Child Forwarding

A child node sends its data to its parent node.

The parent stores the child data and sends it together with its own heartbeat to the Raspberry Pi server.

This allows the Raspberry Pi to know the full network tree.

---

## How the Arduino ESP32 Code Works

The ESP32 Arduino code is written as a universal node program.

This means the same code can be uploaded to every ESP32. A board does not need separate parent code or child code. Each ESP32 decides what to do depending on what WiFi networks it can see.

---

### Main Arduino File

The main `.ino` file is very small.

Example:

```cpp
#include "Config_And_State.h"

void setup() {
  nodeSetup();
}

void loop() {
  nodeLoop();
}
```

The real logic is inside the other `.cpp` files.

The `.ino` file only starts the node setup once and then keeps running the node loop forever.

---

### Config_And_State.h

This file contains the shared configuration, global variables, and function declarations.

It usually contains:

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>
```

It also contains important project settings such as:

```cpp
const char* MAIN_WIFI_SSID = "Your WiFi Name";
const char* MAIN_WIFI_PASSWORD = "Your WiFi Password";
String raspberryPiUrl = "http://192.168.1.100";
```

It also stores the current node state.

Example variables:

```cpp
String node_id = "Node 999";
String parent_node = "";
String parent_ip = "";
bool connectedToParent = false;
bool useMainParent = true;
```

The node starts as:

```text
Node 999
```

This is only a temporary name.

After connection, the node asks for a real unique ID such as:

```text
Node 1
```

or:

```text
Node 2
```

---

## The `useMainParent` Variable

The variable `useMainParent` is a debug/control variable used to decide how the ESP32 searches for its parent.

It is usually found inside `Config_And_State.h`.

Example:

```cpp
bool useMainParent = true;
```

or:

```cpp
bool useMainParent = false;
```

This variable does not change the physical ESP32 board. It only changes the connection behavior of the code.

---

### When `useMainParent = true`

When `useMainParent` is set to `true`, the ESP32 is allowed to connect directly to the Raspberry Pi main WiFi network.

This is usually used for the first/root node.

Example:

```cpp
bool useMainParent = true;
```

Behavior:

```text
ESP32 starts
Searches for Raspberry Pi / main WiFi
Connects to main WiFi if available
Asks Raspberry Pi for unique node ID
Becomes something like Node 1
Starts its own AP as Node 1
Sends heartbeat directly to Raspberry Pi
```

Example structure:

```text
Raspberry Pi
└── Node 1
```

In this mode, the node can become a direct child of the Raspberry Pi.

This is useful when testing the first ESP32 node.

---

### When `useMainParent = false`

When `useMainParent` is set to `false`, the ESP32 skips the Raspberry Pi main WiFi and searches only for another ESP32 node.

This is usually used for testing child nodes.

Example:

```cpp
bool useMainParent = false;
```

Behavior:

```text
ESP32 starts
Does not connect directly to Raspberry Pi
Searches for Node x WiFi
Connects to the strongest available ESP32 node
Asks that parent node for a unique node ID
Becomes something like Node 2
Starts its own AP as Node 2
Sends heartbeat to its parent node
```

Example structure:

```text
Raspberry Pi
└── Node 1
    └── Node 2
```

In this mode, the node becomes a child of another ESP32 node.

This is useful when testing if child-to-parent communication works.

---

### Why `useMainParent` Was Useful

The variable was useful for debugging.

It allowed manual control:

```text
useMainParent = true
```

Use this ESP32 as a direct Raspberry Pi node.

```text
useMainParent = false
```

Force this ESP32 to connect through another node.

This helped test both situations:

```text
Raspberry Pi → Node 1
```

and:

```text
Raspberry Pi → Node 1 → Node 2
```

Without this variable, every ESP32 would try to make the best automatic decision by itself.

---

### Problem With `useMainParent`

The problem is that it requires manual configuration.

For example, if one ESP32 has:

```cpp
bool useMainParent = true;
```

it tries to connect to the Raspberry Pi.

If another ESP32 has:

```cpp
bool useMainParent = false;
```

it skips the Raspberry Pi and searches for another node.

This works for debugging, but it is not fully automatic.

A better final system should not need this variable.

---

### Better Automatic Logic Without `useMainParent`

The improved logic is:

```text
Scan all nearby WiFi networks

If Raspberry Pi WiFi is visible and strong enough:
    connect to Raspberry Pi

Else:
    find the strongest Node x WiFi
    connect to that node
```

Example:

```text
Raspberry Pi WiFi RSSI: -40
Node 1 RSSI: -55
```

The ESP32 should connect to Raspberry Pi because `-40` is stronger.

Another example:

```text
Raspberry Pi WiFi RSSI: -85
Node 1 RSSI: -45
```

The ESP32 should connect to `Node 1` because the Raspberry Pi signal is weak and `Node 1` is closer.

In the final automatic version, `useMainParent` can be removed completely.

The ESP32 should decide automatically using RSSI.

---

### Simple Meaning of `useMainParent`

Simple explanation:

```text
useMainParent = true
```

means:

```text
Try to connect to Raspberry Pi directly.
```

```text
useMainParent = false
```

means:

```text
Do not connect to Raspberry Pi directly.
Connect to another ESP32 node instead.
```

---

## nodeSetup()

The `nodeSetup()` function runs once when the ESP32 starts.

It does these main steps:

```text
1. Start Serial Monitor
2. Set the temporary node ID to Node 999
3. Search for a parent connection
4. Connect to Raspberry Pi WiFi or another Node x WiFi
5. Ask for a unique node ID
6. Start its own ESP32 access point
7. Start the internal web server
```

Example startup flow:

```text
ESP32 starts
Temporary name = Node 999
Search for Raspberry Pi WiFi or Node x WiFi
Connect to the best parent
Ask for unique node ID
Start own AP with new node ID
Start server routes
```

---

## nodeLoop()

The `nodeLoop()` function runs forever.

It keeps the ESP32 alive and sending data.

It usually does these jobs:

```text
1. Handle web server requests
2. Remove old child data
3. Send heartbeat data
4. Send child data when needed
5. Keep the node connected
```

Example loop logic:

```text
while ESP32 is powered on:
    listen for child requests
    receive child heartbeat data
    remove offline children
    send own heartbeat
    forward children data to parent or Raspberry Pi
```

---

## WiFi Connection Logic

The WiFi logic decides where the ESP32 should connect.

The node can connect to:

```text
1. Raspberry Pi main WiFi
2. Another ESP32 node
```

The automatic logic should be:

```text
If Raspberry Pi WiFi is visible and signal is good:
    connect directly to Raspberry Pi
else:
    connect to the strongest nearby Node x WiFi
```

The ESP32 scans all nearby WiFi networks.

Example scan result:

```text
Found WiFi: Node 1 RSSI: -50
Found WiFi: Node 2 RSSI: -65
Found WiFi: Home WiFi RSSI: -80
```

The best signal is the RSSI value closest to zero.

Example:

```text
Node 1 RSSI -50 is better than Node 2 RSSI -65
```

So the ESP32 chooses:

```text
Node 1
```

---

## Own Access Point

After the ESP32 gets its real node ID, it starts its own WiFi access point.

Example:

```text
Node ID: Node 1
AP SSID: Node 1
```

This lets other ESP32 boards connect to it.

So if another ESP32 cannot reach the Raspberry Pi, it can connect to this node instead.

Example:

```text
Raspberry Pi
└── Node 1
    └── Node 2
```

In this case:

```text
Node 1 connects to Raspberry Pi
Node 2 connects to Node 1
```

---

## Unique Node ID System

Every ESP32 first starts as:

```text
Node 999
```

This prevents problems before the final node name is assigned.

After connecting to a parent, the ESP32 asks for a unique ID.

If connected to Raspberry Pi, it asks the Raspberry Pi server.

If connected to another node, it asks the parent node.

Example request:

```text
GET /request-node-id
```

The server or parent checks which node IDs already exist.

Then it gives the first free node number.

Example:

```text
Node 1 is used
Node 2 is free
Assigned ID = Node 2
```

The ESP32 then changes its name from:

```text
Node 999
```

to:

```text
Node 2
```

Then it starts its own access point as:

```text
SSID: Node 2
```

---

## Internal ESP32 Web Server

Each ESP32 runs a small web server.

This is needed so child nodes can communicate with their parent.

Important routes can include:

```text
/child-heartbeat
/request-node-id
/parent-status
```

---

## /child-heartbeat

This route receives data from a child node.

Example:

```text
POST /child-heartbeat
```

A child sends JSON data to the parent.

Example:

```json
{
  "node_id": "Node 2",
  "parent_node": "Node 1",
  "temperature": 25.0,
  "rssi": -55,
  "children": []
}
```

The parent stores this child data.

Then when the parent sends its heartbeat to the Raspberry Pi, it includes this child inside its `children` list.

---

## /request-node-id

This route gives a child node a unique node ID.

Example:

```text
GET /request-node-id
```

If a new child connects to `Node 1`, it can ask `Node 1` for a free ID.

The parent checks:

```text
Its own node ID
Its children node IDs
Node IDs already assigned
```

Then it returns an available ID.

Example response:

```json
{
  "node_id": "Node 3"
}
```

---

## /parent-status

This route tells a child if the parent is busy.

Example:

```text
GET /parent-status
```

The child can ask:

```text
Is parent busy?
```

If the parent is not busy, the child sends data.

This helps avoid multiple child nodes sending data at the same time.

Example response:

```json
{
  "busy": false
}
```

---

## JSON Creation

The ESP32 creates JSON messages before sending data.

Example own node JSON:

```json
{
  "node_id": "Node 1",
  "parent_node": "Raspberry Pi",
  "temperature": 25.0,
  "rssi": -43,
  "children": []
}
```

If the node has children, it creates a bigger JSON message.

Example:

```json
{
  "node_id": "Node 1",
  "parent_node": "Raspberry Pi",
  "temperature": 25.0,
  "rssi": -43,
  "children": [
    {
      "node_id": "Node 2",
      "parent_node": "Node 1",
      "temperature": 25.0,
      "rssi": -55,
      "children": []
    }
  ]
}
```

---

## Heartbeat System

The heartbeat is the message that tells the parent or Raspberry Pi that the node is still online.

Each node sends a heartbeat every few seconds.

Example:

```text
Node 1 sends heartbeat to Raspberry Pi
Node 2 sends heartbeat to Node 1
Node 3 sends heartbeat to Node 1
```

Then `Node 1` forwards the child data to the Raspberry Pi.

Example final tree:

```text
Raspberry Pi
└── Node 1
    ├── Node 2
    └── Node 3
```

---

## Removing Old Children

If a child stops sending heartbeat messages, the parent removes it from its child list.

Example:

```text
Node 2 stops sending data
Node 1 waits a few seconds
Node 1 removes Node 2 from children
Node 1 sends updated data to Raspberry Pi
```

This keeps the dashboard clean and prevents old offline nodes from staying visible forever.

---

## Parent-Child Data Flow

Example with one parent and one child:

```text
Node 2 reads its data
Node 2 sends data to Node 1
Node 1 stores Node 2 data
Node 1 sends its own data plus Node 2 data to Raspberry Pi
Raspberry Pi updates dashboard
```

JSON flow:

```text
Node 2 JSON
    ↓
Node 1 children list
    ↓
Node 1 heartbeat JSON
    ↓
Raspberry Pi Flask server
    ↓
Dashboard table and tree
```

---

## Full ESP32 Startup Example

Direct Raspberry Pi connection example:

```text
ESP32 starts
Serial monitor starts
node_id = Node 999

Scanning WiFi networks...

Raspberry Pi WiFi found
Connecting to Raspberry Pi...

Connected
Asking for unique node ID...

Assigned node_id = Node 1

Starting own AP...
AP name = Node 1

Starting node web server...
Sending heartbeat to Raspberry Pi...
```

Child node example:

```text
ESP32 starts
node_id = Node 999

Scanning WiFi networks...

Raspberry Pi WiFi weak or not visible
Searching for Node x WiFi...

Found Node 1 RSSI -50
Connecting to Node 1...

Connected
Asking Node 1 for unique node ID...

Assigned node_id = Node 2

Starting own AP...
AP name = Node 2

Sending heartbeat to Node 1...
```

---

## Why the Code Is Split Into Files

The ESP32 code is split because the project became too long for one file.

Each file has a clear job.

Example:

```text
esp_node.ino
```

Starts the program.

```text
Config_And_State.h
```

Stores includes, settings, variables, and function declarations.

```text
10_Node_Setup_And_Loop.cpp
```

Contains `nodeSetup()` and `nodeLoop()`.

```text
20_Json_Functions.cpp
```

Creates and reads JSON data.

```text
30_Server_Handlers.cpp
```

Handles web server routes like child heartbeat and node ID requests.

```text
40_WiFi_Functions.cpp
```

Handles WiFi scanning, parent selection, and connection.

```text
50_Heartbeat_Functions.cpp
```

Sends heartbeat data to the Raspberry Pi or parent node.

This makes the project easier to read, fix, and improve.

---

## Raspberry Pi Server Setup

### 1. Copy the server folder to the Raspberry Pi

From your computer:

```bash
scp -r ~/Downloads/sensor_tree_server pi@192.168.1.100:~/
```

If you want to copy the folder from the Raspberry Pi back to your Downloads folder:

```bash
scp -r pi@192.168.1.100:~/sensor_tree_server ~/Downloads/
```

---

### 2. SSH into the Raspberry Pi

```bash
ssh pi@192.168.1.100
```

---

### 3. Go to the project folder

```bash
cd ~/sensor_tree_server
```

---

### 4. Install Flask

```bash
sudo apt update
sudo apt install python3-flask -y
```

---

### 5. Run the server

```bash
sudo python3 server.py
```

The server should show something like:

```text
Running on http://192.168.1.100:80
```

---

## Web Dashboard

Open this in your browser:

```text
http://192.168.1.100
```

Available pages:

```text
http://192.168.1.100/
http://192.168.1.100/nodes
http://192.168.1.100/tree
```

---

## API Routes

### Home Page

```text
GET /
```

Shows the main dashboard page.

---

### Active Nodes Page

```text
GET /nodes
```

Shows the active nodes table.

---

### Tree Page

```text
GET /tree
```

Shows the node tree.

---

### Nodes API

```text
GET /api/nodes
```

Returns all active nodes as JSON.

---

### Tree API

```text
GET /api/tree
```

Returns the node structure as a tree.

Example:

```json
{
  "node_id": "Raspberry Pi",
  "children": [
    {
      "node_id": "Node 1",
      "parent_node": "Raspberry Pi",
      "temperature": 25.0,
      "rssi": -43,
      "children": [
        {
          "node_id": "Node 2",
          "parent_node": "Node 1",
          "temperature": 25.0,
          "rssi": -55,
          "children": []
        }
      ]
    }
  ]
}
```

---

### Heartbeat

```text
POST /heartbeat
```

Used by ESP32 nodes to send data to the Raspberry Pi.

---

### Node ID Request

```text
GET /request-node-id
```

Used by ESP32 nodes to ask for a unique node ID.

---

## ESP32 Setup

### Required Libraries

Install these libraries in Arduino IDE:

- WiFi
- WebServer
- HTTPClient
- ArduinoJson

---

### Uploading Code

Open the ESP32 project folder in Arduino IDE.

The main `.ino` file should contain:

```cpp
#include "Config_And_State.h"

void setup() {
  nodeSetup();
}

void loop() {
  nodeLoop();
}
```

The rest of the logic should be split into `.cpp` and `.h` files.

---

## Important Configuration

Inside `Config_And_State.h`, check your WiFi settings:

```cpp
const char* MAIN_WIFI_SSID = "Your WiFi Name";
const char* MAIN_WIFI_PASSWORD = "Your WiFi Password";
```

Also check the Raspberry Pi server IP:

```cpp
String raspberryPiUrl = "http://192.168.1.100";
```

For debugging, you may also see:

```cpp
bool useMainParent = true;
```

or:

```cpp
bool useMainParent = false;
```

This variable controls whether the ESP32 tries to connect directly to the Raspberry Pi WiFi or only searches for another ESP32 node.

---

## RSSI Explanation

RSSI means Received Signal Strength Indicator.

It shows how strong the WiFi signal is.

The value is usually negative.

Better signal:

```text
-30 dBm
```

Weak signal:

```text
-80 dBm
```

Rule:

```text
The higher the RSSI number, the better the signal.
```

Example:

```text
-40 is better than -70
```

---

## Common Problems

### ESP32 does not connect to parent node

Check that the parent node has started its own access point.

You should see something like this in the serial monitor:

```text
AP started: Node 1
```

Also make sure the child is not ignoring the parent because it thinks the parent has the same node ID.

---

### Child ESP32 connects to the wrong place

Check the value of:

```cpp
bool useMainParent = true;
```

If it is `true`, the ESP32 may try to connect directly to the Raspberry Pi.

If it is `false`, the ESP32 skips the Raspberry Pi and searches for a `Node x` WiFi.

For the final automatic version, this variable should be removed and replaced with RSSI-based parent selection.

---

### Raspberry Pi does not show child nodes

Make sure the child sends its data to the parent.

Also make sure the parent forwards its children list to the Raspberry Pi.

The parent heartbeat must include:

```json
"children": []
```

or:

```json
"children": [
  {
    "node_id": "Node 2",
    "parent_node": "Node 1"
  }
]
```

---

### Flask server says port 80 is already in use

Find what is using port 80:

```bash
sudo lsof -i :80
```

Stop the service that is using it, or change the Flask server port.

---

### SSH warning: remote host identification has changed

Remove the old saved key:

```bash
ssh-keygen -f '/home/smavitzis/.ssh/known_hosts' -R '192.168.1.100'
```

Then try SSH again:

```bash
ssh pi@192.168.1.100
```

---

## Static IP

The Raspberry Pi should use a static IP so the ESP32 nodes always know where to send data.

Recommended IP:

```text
192.168.1.100
```

Example using `nmcli`:

```bash
sudo nmcli connection modify "Wired connection 1" ipv4.addresses 192.168.1.100/24
sudo nmcli connection modify "Wired connection 1" ipv4.gateway 192.168.1.1
sudo nmcli connection modify "Wired connection 1" ipv4.dns "8.8.8.8 1.1.1.1"
sudo nmcli connection modify "Wired connection 1" ipv4.method manual
sudo nmcli connection up "Wired connection 1"
```

---

## Example Serial Monitor Output

Direct Raspberry Pi node example:

```text
================================
ESP32 UNIVERSAL NODE
================================
Temporary starting node_id = Node 999
useMainParent = true

Main parent mode
----------------
This ESP32 is allowed to connect directly to Raspberry Pi.

Searching for parent...
Found Raspberry Pi WiFi
Connected to Raspberry Pi
Asking for unique node ID...
Assigned node ID: Node 1
Starting own AP: Node 1
Sending heartbeat...
```

Child node example:

```text
================================
ESP32 UNIVERSAL NODE
================================
Temporary starting node_id = Node 999
useMainParent = false

Parent connection mode
----------------------
This ESP32 searches for a Node parent.

Searching for strongest Node x WiFi...
Found WiFi: Node 1 RSSI: -50
Strongest parent selected: Node 1
Connected to parent node
Asking for unique node ID...
Assigned node ID: Node 2
Starting own AP: Node 2
Sending heartbeat to parent...
```

Automatic final version example:

```text
================================
ESP32 UNIVERSAL NODE
================================
Temporary starting node_id = Node 999

Scanning all nearby WiFi networks...

Found Raspberry Pi WiFi RSSI: -82
Found WiFi: Node 1 RSSI: -45

Raspberry Pi signal is weak.
Closest parent selected: Node 1

Connected to parent node
Asking for unique node ID...
Assigned node ID: Node 2
Starting own AP: Node 2
Sending heartbeat to parent...
```

---

## Future Improvements

- Remove `useMainParent`
- Use fully automatic RSSI-based parent selection
- Better web tree design
- Signal strength colors in the table
- Node status badges
- Battery level reporting
- Real temperature sensor support
- Node logs page
- Automatic parent switching
- Better parent selection algorithm
- Store node history in a database
- Export node data to CSV
- Add authentication to dashboard
- Add graph view using JavaScript
- Add uptime for each node

---

## Project Type

This is an embedded systems and IoT project.

It combines:

- Embedded programming
- Wireless networking
- Web server development
- Sensor data collection
- Raspberry Pi server management
- ESP32 firmware development

---

## Author

Created by Stamos Mavi.
