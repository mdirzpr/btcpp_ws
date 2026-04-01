# BehaviorTree.CPP workspace

<p align="center">
  <img src="./images/BTCPP_WS.png" alt="BehaviorTree.CPP" width="35%"/>
</p>

## Overview

This workspace is designed for the development and testing of behavior trees using [BehaviorTree.CPP (BTCPP)](https://www.behaviortree.dev/), a powerful and flexible C++ library for building behavior trees. Behavior trees are an AI architecture widely used in robotics and games to manage complex decision-making processes. They are structured as hierarchical trees where each node represents a task, decision, or action, allowing for modular, reusable, and maintainable code.

## BehaviorTree.CPP

BehaviorTree.CPP is an open-source library that facilitates the creation and execution of behavior trees in C++. It provides a highly customizable and easy-to-use framework with various features such as asynchronous actions, reactive behaviors, and integration with other middleware and libraries. For more details on BehaviorTree.CPP, you can visit the [official website](https://www.behaviortree.dev/) or check out the [GitHub repository](https://github.com/BehaviorTree/BehaviorTree.CPP).

## Docker-based Development Environment

This workspace includes a Docker-based environment tailored for developing with BehaviorTree.CPP. The [Docker](https://www.docker.com/) environment ensures consistency across different development setups and simplifies dependency management.

---

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/mdirzpr/btcpp_ws.git
cd btcpp_ws
```

### 2. (Optional) Enable Groot2 visualization

Run this on your **host** before starting the container — it allows the container to open windows on your display:

```bash
xhost +local:docker
```

### 3. Build the Docker image

```bash
docker compose build
```

### 4. Start a dev container

```bash
docker compose run dev bash
```

---

## Build & Run `bt_example`

All commands below run **inside the container**.

### Build

```bash
cmake -S /dev_ws/src/bt_example -B /dev_ws/build/bt_example -DCMAKE_BUILD_TYPE=Release
cmake --build /dev_ws/build/bt_example -- -j$(nproc)
```

### Run the simulation

```bash
/dev_ws/build/bt_example/autonomy_node_sim \
    /dev_ws/src/bt_example/bt_structures/simulation_demo.xml
```

This runs the industrial manufacturing workflow:
**Transport → Quality Assurance → Storage**

### Visualize with Groot2

While the simulation is running, open a second terminal in the container and launch Groot2:

```bash
~/Groot2.AppImage &
```

Groot2 connects automatically on ZMQ port **1668**.

> **Note:** If Groot2 fails with `libOpenGL.so.0: cannot open shared object file`, install the missing library:
> ```bash
> sudo apt-get install -y libopengl0
> ```
> This is only needed once per container session. The next `docker compose build` will include it automatically.

---

## Quick Test (build + Groot2 + run in one command)

From inside the container, run:

```bash
bash /dev_ws/test.sh
```

Or from the **host** workspace root:

```bash
docker compose run dev bash /dev_ws/test.sh
```

`test.sh` will:
1. Build `bt_example`
2. Launch Groot2 in the background (if available)
3. Run the simulation
4. Shut down Groot2 when the simulation exits

---

## Project Structure

```
btcpp_ws/
├── bt_example/
│   ├── include/action_nodes.h       # All custom BT node declarations
│   ├── src/
│   │   ├── action_nodes.cpp         # Node implementations
│   │   └── autonomy_node_sim.cpp    # main() — registers nodes, loads tree, ticks
│   ├── bt_structures/
│   │   ├── simulation_demo.xml      # Main manufacturing workflow tree
│   │   └── test.xml                 # Minimal smoke-test tree (AlwaysSuccess)
│   └── CMakeLists.txt
├── docker/
│   └── Dockerfile
├── docker-compose.yaml
└── test.sh                          # One-shot build + Groot2 + run script
```

---

### Join the Community

For discussions, updates, and networking with other BehaviorTree.CPP users, consider joining the [BehaviorTree LinkedIn Group](https://www.linkedin.com/groups/13022577/).

### Developer and maintainer
- [Mahdi Rezapour](https://github.com/mdirzpr)
- Email: mahdi.rezapour@smartfactory.de
