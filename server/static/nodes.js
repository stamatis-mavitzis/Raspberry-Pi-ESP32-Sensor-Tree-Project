async function loadNodes() {
    try {
        const response = await fetch("/api/nodes");

        if (!response.ok) {
            console.log("Server error:", response.status);
            return;
        }

        const data = await response.json();

        const tableBody = document.getElementById("nodesTableBody");
        const nodeCount = document.getElementById("nodeCount");

        if (!tableBody || !nodeCount) {
            console.log("Missing nodesTableBody or nodeCount in nodes.html");
            return;
        }

        tableBody.innerHTML = "";

        nodeCount.textContent = "Active nodes: " + data.count;

        const rawNodes = data.raw || {};

        const sortedNodeIds = Object.keys(rawNodes).sort((a, b) => {
            const numA = parseInt(a.replace("Node ", ""));
            const numB = parseInt(b.replace("Node ", ""));

            if (isNaN(numA)) return 1;
            if (isNaN(numB)) return -1;

            return numA - numB;
        });

        for (const nodeId of sortedNodeIds) {
            const node = rawNodes[nodeId];

            const row = document.createElement("tr");

            const nodeIdCell = document.createElement("td");
            nodeIdCell.textContent = emptyToDash(node.node_id);

            const parentCell = document.createElement("td");
            parentCell.textContent = emptyToDash(node.parent_node);

            const temperatureCell = document.createElement("td");

            if (isEmpty(node.temperature)) {
                temperatureCell.textContent = "-";
            } else {
                temperatureCell.textContent = node.temperature + " °C";
            }

            const rssiCell = document.createElement("td");

            if (isEmpty(node.rssi)) {
                rssiCell.textContent = "-";
            } else {
                rssiCell.textContent = node.rssi + " dBm";
            }

            const childrenCell = document.createElement("td");

            if (isEmpty(node.children)) {
                childrenCell.textContent = "-";
            } else {
                childrenCell.textContent = node.children;
            }

            const lastSeenCell = document.createElement("td");
            lastSeenCell.textContent = emptyToDash(node.last_seen);

            row.appendChild(nodeIdCell);
            row.appendChild(parentCell);
            row.appendChild(temperatureCell);
            row.appendChild(rssiCell);
            row.appendChild(childrenCell);
            row.appendChild(lastSeenCell);

            tableBody.appendChild(row);
        }
    } catch (error) {
        console.log("Error loading nodes:", error);
    }
}

function isEmpty(value) {
    return (
        value === null ||
        value === undefined ||
        value === "" ||
        value === "[]" ||
        value.length === 0
    );
}

function emptyToDash(value) {
    if (isEmpty(value)) {
        return "-";
    }

    return value;
}

loadNodes();
setInterval(loadNodes, 1000);














