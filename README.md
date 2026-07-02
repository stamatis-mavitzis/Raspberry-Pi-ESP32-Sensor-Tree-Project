<div align="center">

# ESP32 Raspberry Pi Sensor Tree Project

### Wireless ESP32 Sensor Network with Raspberry Pi Flask Dashboard

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

The **ESP32 Raspberry Pi Sensor Tree Project** is an IoT project that creates a wireless sensor network using ESP32 microcontrollers and a Raspberry Pi server.

The ESP32 devices work as wireless nodes. Each node can send its own data, receive data from child nodes, and forward that information to its parent. The Raspberry Pi runs a Flask server that receives the node data and displays it in a browser dashboard.

The project is designed around a **tree topology**, where the Raspberry Pi is the root and the ESP32 devices can become parent or child nodes.

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

This makes the project useful for learning embedded systems, wireless communication, networking, backend development, frontend development, and IoT system design.

---

## Project Goal

The goal of this project is to build a wireless sensor tree where ESP32 nodes can communicate with each other and send their data to a Raspberry Pi dashboard.

The project demonstrates:

- How ESP32 devices can connect over WiFi
- How nodes can form a parent-child network
- How data can be sent using HTTP requests
- How JSON can be used for data exchange
- How a Raspberry Pi can act as a local IoT server
- How a Flask backend can receive and organize node data
- How a web dashboard can display live IoT information

---

## Languages and Technologies Used

| Language / Technology | Used For |
|---|---|
| **C++ / Arduino** | ESP32 firmware, WiFi logic, node logic, HTTP requests, local ESP32 server |
| **Python** | Flask backend server on the Raspberry Pi |
| **Flask** | Web server, API routes, dashboard page rendering |
| **HTML** | Dashboard page structure |
| **CSS** | Dashboard layout, styling, buttons, table design |
| **JavaScript** | Fetching live API data and updating the dashboard |
| **JSON** | Data format between ESP32 nodes and the Raspberry Pi |
| **Linux / Bash** | Running the server, copying files, system setup |
| **Git / GitHub** | Version control and project hosting |

---

## Main Features

### ESP32 Features

- Universal ESP32 node firmware
- Each ESP32 can act as a parent or child node
- Starts its own WiFi access point
- Searches for available parent nodes
- Connects to the Raspberry Pi or another ESP32
- Sends heartbeat data
- Receives heartbeat data from child nodes
- Stores child data temporarily
- Forwards child data to the parent
- Requests a unique node ID
- Reports signal strength using RSSI
- Supports temperature value reporting
- Uses modular Arduino/C++ files for better organization

### Raspberry Pi Server Features

- Python Flask server
- Receives heartbeat data from ESP32 nodes
- Stores active node information
- Removes nodes that stop sending data
- Provides API endpoints for the dashboard
- Serves HTML pages
- Builds a tree view from node relationships
- Shows the latest state of the wireless network

### Dashboard Features

- Home page
- Active nodes table
- Tree view page
- Live node count
- Parent node display
- RSSI display
- Temperature display
- Child node display
- Last seen time
- Automatic frontend updates using JavaScript

---

## System Architecture

The system is split into two main parts:

1. **ESP32 firmware**
2. **Raspberry Pi Flask server**

The ESP32 firmware handles wireless node behavior. The Raspberry Pi server handles data collection, APIs, and the web dashboard.

```text
ESP32 Node
   |
   | HTTP / JSON
   v
ESP32 Parent Node
   |
   | HTTP / JSON
   v
Raspberry Pi Flask Server
   |
   | API / JSON
   v
Web Dashboard
```

---

## How the Network Works

Each ESP32 node follows this general logic:

1. The ESP32 starts.
2. It starts serial communication for debugging.
3. It starts its own WiFi access point.
4. It searches for a parent.
5. It connects either to the Raspberry Pi network or another ESP32 node.
6. It asks for a unique node ID.
7. It starts a local web server.
8. It sends heartbeat data to its parent.
9. If it has child nodes, it forwards their data too.
10. The Raspberry Pi receives the data and updates the dashboard.

---

## Data Flow

A simple node sends its own heartbeat directly to the Raspberry Pi:

```text
Node 1 -> Raspberry Pi
```

A child node sends data to a parent node first:

```text
Node 2 -> Node 1 -> Raspberry Pi
```

A larger tree may look like this:

```text
Node 5 -> Node 3 -> Node 1 -> Raspberry Pi
```

This allows nodes that cannot directly reach the Raspberry Pi to still send data through another ESP32.

---

## Project Structure

The project is separated into two main root folders:

- `nodes/` contains the ESP32 Arduino firmware.
- `server/` contains the Raspberry Pi Flask server and dashboard.

The Arduino code is not inside the server folder. The server and the ESP32 node firmware are separate parts of the same project.

```text
Raspberry-Pi-ESP32-Sensor-Tree-Project/
│
├── nodes/
│   ├── esp_node.ino
│   ├── Config_And_State.h
│   ├── 01_Json_Helpers.cpp
│   ├── 02_Child_Storage.cpp
│   ├── 03_Tree_Builder.cpp
│   ├── 04_Parent_Status.cpp
│   ├── 05_Node_Id.cpp
│   ├── 06_Node_Server.cpp
│   ├── 07_WiFi_Connection.cpp
│   ├── 08_Heartbeat.cpp
│   ├── 09_Main_Node_Logic.cpp
│   └── 10_Node_Setup_And_Loop.cpp
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

## Arduino Code Explanation

The Arduino code is split into multiple files instead of keeping everything inside one large `.ino` file.

This makes the code easier to read, easier to debug, and easier to expand later.

---

## esp_node.ino

This is the main Arduino sketch file.

It only contains the normal Arduino `setup()` and `loop()` functions.

```cpp
void setup() {
    nodeSetup();
}

void loop() {
    nodeLoop();
}
```

The reason for keeping this file small is organization. The real logic is placed in separate `.cpp` files.

`setup()` runs once when the ESP32 starts.

`loop()` runs forever while the ESP32 is powered.

Instead of writing all code directly inside these functions, the project uses:

```cpp
nodeSetup();
nodeLoop();
```

This keeps the main sketch clean.

---

## Config_And_State.h

This file is one of the most important files in the Arduino code.

It contains:

- Required libraries
- Function declarations
- Global variables
- Configuration values
- Shared state used by multiple `.cpp` files

Typical includes are:

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>
```

These libraries are used for:

| Library | Purpose |
|---|---|
| `Arduino.h` | Basic Arduino functions such as `Serial`, `delay`, `millis` |
| `WiFi.h` | ESP32 WiFi connection and scanning |
| `WebServer.h` | Small HTTP server running on the ESP32 |
| `HTTPClient.h` | Sending HTTP requests to parent nodes or Raspberry Pi |
| `ArduinoJson.h` | Creating and parsing JSON data |
| `vector` | Storing child node data dynamically |

This file also declares functions before they are used in other files. This prevents compile errors such as:

```text
function was not declared in this scope
```

---

## 01_Json_Helpers.cpp

This file contains helper functions for working with JSON strings.

The ESP32 receives and sends data as JSON. For example:

```json
{
  "node_id": "Node 2",
  "temperature": 25.0,
  "rssi": -45
}
```

JSON helper functions are useful because the ESP32 often needs to extract specific values from received data.

Example responsibilities:

- Extract a string value from JSON
- Clean spaces around JSON text
- Check if a value exists
- Prepare text before parsing

This keeps JSON-related code separate from WiFi and heartbeat code.

---

## 02_Child_Storage.cpp

This file handles child node storage.

When a parent ESP32 receives data from a child ESP32, it needs to remember that child before sending the data upward.

For example:

```text
Node 2 sends data to Node 1
Node 1 stores Node 2 data
Node 1 later forwards Node 2 data to Raspberry Pi
```

This file is responsible for:

- Saving child JSON data
- Updating existing child data
- Removing old child data
- Keeping the children list clean

This is important because a parent node may have more than one child.

Example:

```text
Node 1
├── Node 2
├── Node 3
└── Node 4
```

The parent must store the latest data from each child.

---

## 03_Tree_Builder.cpp

This file builds the JSON tree structure for a node.

A node may need to report:

- Its own ID
- Its parent ID
- Its temperature
- Its RSSI
- Its children

Example output:

```json
{
  "node_id": "Node 1",
  "parent_node": "Raspberry Pi",
  "temperature": 25.0,
  "rssi": -40,
  "children": [
    {
      "node_id": "Node 2",
      "parent_node": "Node 1",
      "temperature": 24.5,
      "rssi": -55,
      "children": []
    }
  ]
}
```

This allows the Raspberry Pi to understand the network structure.

---

## 04_Parent_Status.cpp

This file is used to check if a parent node is busy.

This is useful because a child node should not send data while the parent is already processing another request.

The logic helps reduce communication problems between child and parent nodes.

Example idea:

```text
Child asks: "Parent, are you busy?"
Parent replies: "No"
Child sends heartbeat data
```

This improves reliability when multiple child nodes try to send data.

---

## 05_Node_Id.cpp

This file handles unique node ID logic.

Each ESP32 needs a unique name such as:

```text
Node 1
Node 2
Node 3
```

Unique names are important because the Raspberry Pi dashboard must know which node is which.

Without unique names, two ESP32 devices could both appear as `Node 1`, causing wrong data in the dashboard.

This file handles:

- Asking for a unique node ID
- Waiting until a unique ID is available
- Avoiding duplicate node names
- Starting from a default temporary name such as `Node 999`

The temporary node name is useful while the node is still connecting and has not yet received its final ID.

---

## 06_Node_Server.cpp

This file starts a small web server on each ESP32 node.

The ESP32 server can receive requests from child nodes.

Important endpoints can include:

| Endpoint | Purpose |
|---|---|
| `/child_heartbeat` | Receives heartbeat data from child nodes |
| `/node_id_request` | Gives or helps assign a node ID |
| `/parent_status` | Tells a child if the parent is busy |

This allows an ESP32 parent node to behave like a small local server.

Without this, child nodes would not be able to send their data to parent nodes.

---

## 07_WiFi_Connection.cpp

This file contains the WiFi connection logic.

It is responsible for:

- Starting the ESP32 access point
- Scanning nearby WiFi networks
- Detecting ESP32 node networks
- Connecting to the Raspberry Pi WiFi
- Connecting to another ESP32 node
- Selecting the best parent based on RSSI

RSSI is used to estimate signal strength.

Example:

```text
-40 dBm is stronger than -80 dBm
```

So if the ESP32 sees:

```text
Node 1 RSSI: -45
Node 2 RSSI: -70
```

It should prefer `Node 1` because `-45` is closer to zero and therefore stronger.

---

## 08_Heartbeat.cpp

This file handles heartbeat communication.

A heartbeat is a small message that a node sends repeatedly to say:

```text
I am alive, here is my latest data.
```

Example heartbeat JSON:

```json
{
  "node_id": "Node 2",
  "parent_node": "Node 1",
  "temperature": 25.0,
  "rssi": -45,
  "children": []
}
```

The heartbeat system allows the Raspberry Pi to know:

- Which nodes are online
- Which nodes are offline
- What parent each node has
- What data each node is reporting
- What children each node has

If a node stops sending heartbeat data, the server removes it after a timeout.

---

## 09_Main_Node_Logic.cpp

This file contains the main runtime behavior of the ESP32 node.

It connects the smaller parts of the system together.

It controls logic such as:

- Should the node connect to the main Raspberry Pi network?
- Should the node search for another ESP32 parent?
- Has the node received a unique ID?
- Should it send heartbeat data now?
- Should it forward child data?
- Should it retry connection?

This file acts like the "brain" of the ESP32 node.

---

## 10_Node_Setup_And_Loop.cpp

This file contains the real setup and loop logic for the node.

`nodeSetup()` is called once from `setup()`.

It usually handles:

- Serial start
- Startup messages
- WiFi setup
- Access point setup
- Parent connection
- Unique node ID request
- ESP32 server start

`nodeLoop()` is called repeatedly from `loop()`.

It usually handles:

- Handling incoming web server requests
- Sending heartbeat data
- Checking timing with `millis()`
- Forwarding child data
- Keeping the node active

This separation makes it easy to add more sensors later.

For example, you can add a sensor reading function and call it inside `nodeLoop()`.

---

## Raspberry Pi Server Code Explanation

The Raspberry Pi server is written in Python using Flask.

The server receives HTTP requests from ESP32 nodes and provides dashboard pages for the browser.

The main server file is:

```text
server/server.py
```

---

## server.py

The Flask server usually contains:

- Flask app creation
- Dictionary for storing nodes
- Timeout logic
- Routes for pages
- Routes for APIs
- Route for heartbeat data

Example Flask setup:

```python
from flask import Flask, request, jsonify, render_template
import time

app = Flask(__name__)

nodes = {}
```

The `nodes` dictionary stores the current active nodes.

Example:

```python
nodes = {
    "Node 1": {
        "node_id": "Node 1",
        "parent_node": "Raspberry Pi",
        "temperature": 25.0,
        "rssi": -40,
        "children": [],
        "last_seen_timestamp": 123456789
    }
}
```

---

## Heartbeat Route

The heartbeat route receives data from ESP32 nodes.

Example route:

```text
POST /heartbeat
```

When an ESP32 sends heartbeat data, the server updates the node information.

The server stores:

- Node ID
- Parent node
- Temperature
- RSSI
- Children
- Last seen time

This is the main communication route between the ESP32 network and the Raspberry Pi.

---

## Node Timeout Logic

The server removes nodes that stop sending heartbeat data.

This prevents the dashboard from showing old offline nodes forever.

Example logic:

```text
If current time - last seen time > timeout:
    remove node
```

So if a node loses power or disconnects, it will disappear from the dashboard after a few seconds.

---

## API Routes

The dashboard uses API routes to get live data.

Common API routes:

```text
/api/nodes
/api/tree
```

### /api/nodes

This returns the active nodes as JSON.

It is used by `nodes.js` to update the nodes table.

### /api/tree

This returns the parent-child tree structure.

It is used by `tree.js` to draw the tree view.

---

## HTML Templates

The dashboard pages are stored in:

```text
server/templates/
```

Common files:

| File | Purpose |
|---|---|
| `index.html` | Home page with navigation buttons |
| `nodes.html` | Active nodes table page |
| `tree.html` | Tree view page |

Flask renders these files when the user opens the dashboard in a browser.

---

## CSS Styling

The CSS file is stored in:

```text
server/static/style.css
```

It controls the visual appearance of the dashboard.

It can style:

- Page background
- Main content box
- Buttons
- Tables
- Text
- Tree view
- Spacing and layout

This makes the dashboard cleaner and easier to use.

---

## JavaScript Files

The JavaScript files are stored in:

```text
server/static/
```

Common files:

| File | Purpose |
|---|---|
| `nodes.js` | Fetches `/api/nodes` and updates the table |
| `tree.js` | Fetches `/api/tree` and renders the tree |

The JavaScript code allows the dashboard to update without refreshing the full page.

Example idea:

```javascript
fetch("/api/nodes")
    .then(response => response.json())
    .then(data => {
        // update table
    });
```

This creates a live dashboard experience.

---

## Example Node Data

A node may send data like this:

```json
{
  "node_id": "Node 2",
  "parent_node": "Node 1",
  "temperature": 25.0,
  "rssi": -45,
  "children": []
}
```

Explanation:

| Field | Meaning |
|---|---|
| `node_id` | The unique name of the ESP32 node |
| `parent_node` | The parent that this node is connected to |
| `temperature` | Example sensor value |
| `rssi` | WiFi signal strength |
| `children` | List of child nodes connected under this node |

---

## RSSI Explanation

RSSI means **Received Signal Strength Indicator**.

It shows how strong the WiFi signal is.

RSSI values are usually negative.

```text
-30 dBm = Very strong
-50 dBm = Good
-70 dBm = Weak
-90 dBm = Very bad
```

A value closer to zero is better.

Example:

```text
-40 is better than -80
```

The project can use RSSI to choose a better parent node.

---

## Why JSON Is Used

JSON is used because it is simple and works well with both ESP32 and Python.

Example JSON:

```json
{
  "node_id": "Node 1",
  "temperature": 25.0
}
```

Benefits of JSON:

- Easy to read
- Easy to send over HTTP
- Works with ArduinoJson on ESP32
- Works directly with Flask in Python
- Works directly with JavaScript in the browser

---

## Why Flask Is Used

Flask is a lightweight Python web framework.

It is useful for this project because:

- It is simple to set up
- It can receive HTTP POST requests
- It can return JSON APIs
- It can serve HTML pages
- It works well on Raspberry Pi
- It is easy to debug and modify

---

## Why the Code Is Modular

The ESP32 code is split into multiple files so that each file has a clear purpose.

This makes the project easier to understand.

Instead of one huge file, the project is organized by responsibility:

```text
WiFi code       -> 07_WiFi_Connection.cpp
Heartbeat code  -> 08_Heartbeat.cpp
Server code     -> 06_Node_Server.cpp
Node ID code    -> 05_Node_Id.cpp
Setup/loop code -> 10_Node_Setup_And_Loop.cpp
```

This is better for debugging and future development.

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

## How to Upload the Arduino Code

1. Open the Arduino IDE.
2. Open the ESP32 sketch folder.
3. Make sure all `.cpp` and `.h` files are inside the same sketch folder.
4. Select the correct ESP32 board.
5. Select the correct USB port.
6. Upload the code.

The main file is inside the `nodes/` folder:

```text
nodes/esp_node.ino
```

The other `.cpp` and `.h` files must stay inside the same `nodes/` folder so the Arduino IDE can compile them together.

---

## GitHub Upload Commands

From the main project folder:

```bash
cd ~/Downloads/delme
```

Add the important project files:

```bash
git add nodes server README.md
```

Commit:

```bash
git commit -m "Update ESP32 Raspberry Pi sensor tree project"
```

Push:

```bash
git push origin main
```

---

## Troubleshooting

### Git says: not a git repository

This means you are not inside the GitHub project folder.

Go to the folder that contains `.git`:

```bash
cd ~/Downloads/delme
```

Check:

```bash
ls -a
```

You should see:

```text
.git
arduino
server
README.md
```

---

### Flask says: Permission denied

This can happen when using port `80`.

Use:

```bash
sudo python3 server.py
```

Or change the server port to a non-root port such as `5000`.

---

### ESP32 compile error: function was not declared

This usually means the function declaration is missing from `Config_And_State.h`.

Add the function declaration there so other files can see it.

Example:

```cpp
void nodeSetup();
void nodeLoop();
```

---

### ESP32 compile error: header file not found

Make sure the header file is inside the same Arduino sketch folder.

Example:

```text
nodes/
├── esp_node.ino
├── Config_And_State.h
└── other files...
```

---

### Node does not appear on dashboard

Check:

- Raspberry Pi server is running
- ESP32 is connected to WiFi
- ESP32 is sending heartbeat data
- Raspberry Pi IP address is correct
- The heartbeat URL is correct
- The node timeout is not too short

---

## Future Improvements

Possible improvements for the project:

- Add real sensors such as DHT11, DHT22, BMP280, or soil moisture sensors
- Add database storage
- Add temperature history charts
- Add battery voltage monitoring
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
- Add MQTT support
- Add Node-RED integration
- Add OTA firmware updates for ESP32 nodes

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
- Modular code organization

---

## Skills Demonstrated

- Embedded systems programming
- ESP32 WiFi networking
- Arduino C++ development
- Raspberry Pi server deployment
- Python Flask backend development
- REST API communication
- JSON data handling
- Frontend dashboard development
- HTML/CSS/JavaScript
- Linux command-line usage
- Git and GitHub usage
- IoT system architecture
- Parent-child network topology
- Modular code organization
- Debugging embedded and server-side systems

---

## Credits

Developed as an educational IoT project using:

- ESP32 microcontrollers
- Raspberry Pi
- Arduino framework
- Python Flask
- HTML
- CSS
- JavaScript
- JSON communication

---

## License

This project is for educational use.

<div align="center">

---

### ESP32 + Raspberry Pi + Flask + IoT Dashboard

</div>
