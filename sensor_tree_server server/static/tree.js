function createNodeElement(node) {
    const li = document.createElement("li");

    const nodeBox = document.createElement("div");
    nodeBox.className = "tree-node";

    const nodeId = node.node_id || "Unknown";
    const parent = node.parent_node || "Unknown";
    const temperature = node.temperature ?? "-";
    const rssi = node.rssi ?? "-";
    const lastSeen = node.last_seen || "-";

    nodeBox.innerHTML = `
        <strong>${nodeId}</strong>
        <span>Parent: ${parent}</span>
        <span>Temp: ${temperature} °C</span>
        <span>RSSI: ${rssi}</span>
        <span>Last seen: ${lastSeen}</span>
    `;

    li.appendChild(nodeBox);

    if (node.children && node.children.length > 0) {
        const ul = document.createElement("ul");

        node.children.forEach(child => {
            if (typeof child === "object") {
                ul.appendChild(createNodeElement(child));
            } else {
                const childLi = document.createElement("li");

                const childBox = document.createElement("div");
                childBox.className = "tree-node child-missing";
                childBox.innerHTML = `
                    <strong>${child}</strong>
                    <span>Child node detected</span>
                    <span>No full data received yet</span>
                `;

                childLi.appendChild(childBox);
                ul.appendChild(childLi);
            }
        });

        li.appendChild(ul);
    }

    return li;
}


async function loadTree() {
    try {
        const response = await fetch("/api/tree");
        const treeData = await response.json();

        const container = document.getElementById("treeContainer");
        container.innerHTML = "";

        const tree = document.createElement("div");
        tree.className = "tree";

        const ul = document.createElement("ul");
        ul.appendChild(createNodeElement(treeData));

        tree.appendChild(ul);
        container.appendChild(tree);

    } catch (error) {
        console.error("Tree load error:", error);
        document.getElementById("treeContainer").innerHTML = "Could not load tree.";
    }
}

loadTree();
setInterval(loadTree, 1000);