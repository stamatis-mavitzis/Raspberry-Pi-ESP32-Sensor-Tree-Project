from flask import Flask, request, jsonify, render_template
import time
import re

app = Flask(__name__)

nodes = {}

NODE_TIMEOUT_SECONDS = 10
NODE_ID_LEASE_SECONDS = 15

assigned_by_device = {}


def format_children(children):
    child_names = []

    if isinstance(children, list):
        for child in children:
            if isinstance(child, dict):
                child_id = child.get("node_id")
                if child_id:
                    child_names.append(child_id)

            elif isinstance(child, str):
                clean_child = child.replace('"', "").strip()
                if clean_child:
                    child_names.append(clean_child)

    return ", ".join(child_names)


def get_child_id(child):
    if isinstance(child, dict):
        return child.get("node_id", "")

    if isinstance(child, str):
        return child.replace('"', "").strip()

    return ""


def remove_old_nodes():
    current_time = time.time()
    old_node_ids = []

    for node_id, node in list(nodes.items()):
        last_seen_timestamp = node.get("last_seen_timestamp", 0)

        if current_time - last_seen_timestamp > NODE_TIMEOUT_SECONDS:
            old_node_ids.append(node_id)

    for node_id in old_node_ids:
        print(f"Removing offline node: {node_id}")
        del nodes[node_id]


def remove_old_assignments():
    current_time = time.time()
    old_devices = []

    for device_id, info in list(assigned_by_device.items()):
        assigned_time = info.get("assigned_time", 0)

        if current_time - assigned_time > NODE_ID_LEASE_SECONDS:
            old_devices.append(device_id)

    for device_id in old_devices:
        old_node_id = assigned_by_device[device_id]["node_id"]
        print(f"Removing old node id lease: {old_node_id} from {device_id}")
        del assigned_by_device[device_id]


def get_node_number(node_id):
    match = re.match(r"^Node (\d+)$", node_id)

    if not match:
        return None

    return int(match.group(1))


def collect_node_ids_from_tree(node):
    ids = set()

    if not isinstance(node, dict):
        return ids

    node_id = node.get("node_id")

    if node_id:
        ids.add(node_id)

    children = node.get("children", [])

    if isinstance(children, list):
        for child in children:
            if isinstance(child, dict):
                ids.update(collect_node_ids_from_tree(child))

            elif isinstance(child, str):
                child_id = get_child_id(child)
                if child_id:
                    ids.add(child_id)

    return ids


def collect_all_active_node_ids():
    remove_old_nodes()

    ids = set()

    for node in nodes.values():
        ids.update(collect_node_ids_from_tree(node))

    return ids


def get_used_node_numbers():
    remove_old_nodes()
    remove_old_assignments()

    used_numbers = set()

    active_node_ids = collect_all_active_node_ids()

    for node_id in active_node_ids:
        number = get_node_number(node_id)

        if number is not None:
            used_numbers.add(number)

    for info in assigned_by_device.values():
        node_id = info.get("node_id")
        number = get_node_number(node_id)

        if number is not None:
            used_numbers.add(number)

    return used_numbers


def build_node_table_rows():
    remove_old_nodes()

    table_rows = {}
    active_node_ids = set()

    for node_id, node in nodes.items():
        children = node.get("children", [])

        if not isinstance(children, list):
            children = []

        table_rows[node_id] = {
            "node_id": node.get("node_id", node_id),
            "parent_node": node.get("parent_node", ""),
            "temperature": node.get("temperature", ""),
            "rssi": node.get("rssi", ""),
            "children": format_children(children),
            "last_seen": node.get("last_seen", "")
        }

        active_node_ids.add(node_id)

        for child in children:
            child_id = get_child_id(child)

            if not child_id:
                continue

            active_node_ids.add(child_id)

            # If child does not have its own heartbeat yet,
            # create a visible placeholder row for it.
            if child_id not in table_rows:
                table_rows[child_id] = {
                    "node_id": child_id,
                    "parent_node": node_id,
                    "temperature": "",
                    "rssi": "",
                    "children": "",
                    "last_seen": ""
                }

    return table_rows, active_node_ids


@app.route("/")
def index_page():
    return render_template("index.html")


@app.route("/nodes")
def nodes_page():
    return render_template("nodes.html")


@app.route("/tree")
def tree_page():
    return render_template("tree.html")


@app.route("/api/nodes")
def api_nodes():
    table_rows, active_node_ids = build_node_table_rows()

    return jsonify({
        "count": len(active_node_ids),
        "nodes": sorted(list(active_node_ids), key=lambda x: get_node_number(x) or 999999),
        "raw": table_rows
    })


@app.route("/api/tree")
def api_tree():
    remove_old_nodes()

    tree = {
        "node_id": "Raspberry Pi",
        "parent_node": "",
        "temperature": "",
        "rssi": "",
        "last_seen": "",
        "children": []
    }

    # Make a copy of every node
    node_map = {}

    for node_id, node in nodes.items():
        node_copy = dict(node)

        # Always make children a list
        if "children" not in node_copy or not isinstance(node_copy["children"], list):
            node_copy["children"] = []

        # Clear children first.
        # We will rebuild the real tree from parent_node.
        node_copy["children"] = []

        node_map[node_id] = node_copy

    # Attach every node to its real parent
    for node_id, node in node_map.items():
        parent_node = node.get("parent_node", "Raspberry Pi")

        if parent_node == "Raspberry Pi":
            tree["children"].append(node)

        elif parent_node in node_map:
            node_map[parent_node]["children"].append(node)

        else:
            # If parent is missing/offline, show it under Raspberry Pi
            tree["children"].append(node)

    return jsonify(tree)


@app.route("/request_node_id", methods=["POST"])
def request_node_id():
    data = request.get_json()

    if not data:
        return jsonify({"error": "No JSON received"}), 400

    current_node_id = data.get("current_node_id", "")
    parent_node = data.get("parent_node", "")
    device_id = data.get("device_id", "")

    if not device_id:
        return jsonify({"error": "Missing device_id"}), 400

    remove_old_nodes()
    remove_old_assignments()

    if device_id in assigned_by_device:
        node_id = assigned_by_device[device_id]["node_id"]
        assigned_by_device[device_id]["assigned_time"] = time.time()

        print(f"Device {device_id} already has {node_id}")

        return jsonify({
            "status": "ok",
            "node_id": node_id
        })

    used_numbers = get_used_node_numbers()

    number = 1

    while number in used_numbers:
        number += 1

    new_node_id = f"Node {number}"

    assigned_by_device[device_id] = {
        "node_id": new_node_id,
        "assigned_time": time.time()
    }

    print(
        f"Assigned unique node id: {new_node_id} "
        f"to device {device_id}, old name: {current_node_id}, parent: {parent_node}"
    )

    return jsonify({
        "status": "ok",
        "node_id": new_node_id
    })


@app.route("/heartbeat", methods=["POST"])
def heartbeat():
    data = request.get_json()

    if not data:
        return jsonify({"error": "No JSON received"}), 400

    node_id = data.get("node_id")
    parent_node = data.get("parent_node", "Raspberry Pi")
    temperature = data.get("temperature", None)
    rssi = data.get("rssi", None)
    children = data.get("children", [])

    if not node_id:
        return jsonify({"error": "Missing node_id"}), 400

    if not isinstance(children, list):
        children = []

    clean_children = []

    for child in children:
        child_id = get_child_id(child)

        if child_id:
            clean_children.append(child_id)

    nodes[node_id] = {
        "node_id": node_id,
        "parent_node": parent_node,
        "temperature": temperature,
        "rssi": rssi,
        "children": clean_children,
        "last_seen": time.strftime("%H:%M:%S"),
        "last_seen_timestamp": time.time()
    }

    print(
        f"Heartbeat from {node_id}, "
        f"parent: {parent_node}, "
        f"temperature: {temperature}, "
        f"rssi: {rssi}, "
        f"children: {format_children(clean_children)}"
    )

    return jsonify({
        "status": "ok",
        "node_id": node_id
    })


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)