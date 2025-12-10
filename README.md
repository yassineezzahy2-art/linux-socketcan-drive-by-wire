# Linux SocketCAN Drive-by-Wire Simulator (v2.0 Real-Time)

## 🚗 Project Overview
This project simulates a **Real-Time Automotive Control System** using Linux SocketCAN. Unlike standard blocking simulations, this version implements **Asynchronous I/O** to achieve a 100Hz control loop, mimicking the cyclic execution of a real Electronic Control Unit (ECU).

The system consists of two nodes communicating via a virtual CAN bus:
1.  **Engine & Body Controller (Sender):** Simulates engine physics (inertia/friction) and door state logic.
2.  **Digital Instrument Cluster (Receiver):** Decodes CAN frames and renders a dynamic dashboard with warnings.

## ⚡ Key Features (v2.0)
* **Real-Time Control Loop:** Implemented `termios` and `fcntl` for non-blocking keyboard input. No "Enter" key required.
* **Physics Simulation:** Engine RPM decays naturally due to simulated friction when throttle is released.
* **State Machine Logic:** Implemented a **Debounced Toggle Switch** for the door control to prevent signal flickering.
* **Bit-Packed CAN Frames:** Optimized bandwidth by packing RPM (16-bit), Temperature (8-bit), and Door Status (1-bit) into a single 4-byte payload.
* **Dynamic Visualization:** Receiver uses ANSI escape codes for a flicker-free, refreshing UI.

## 🛠️ Tech Stack
* **Language:** C++ (Low-level system programming)
* **Protocol:** CAN (Controller Area Network) - Raw Sockets
* **OS APIs:** `<sys/socket.h>`, `<linux/can.h>`, `<termios.h>` (Terminal Control)
* **Architecture:** Cyclic Execution Model (10ms Loop)

## 🎮 Controls
Run the Engine node and use the following keys:
* **[W]** - Accelerate (Increase RPM)
* **[S]** - Brake (Decrease RPM)
* **[D]** - Toggle Door Open/Closed (includes debounce logic)
* **[Q]** - Quit Simulation

## 🚀 How to Run

### 1. Setup Virtual CAN
```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
2. Compile
Bash

make
# OR manually:
g++ sender.cpp -o engine_node
g++ receiver.cpp -o cluster_node
3. Execute
Open two terminal windows:

Terminal 1 (Dashboard):

Bash

./cluster_node
Terminal 2 (Engine):

Bash

./engine_node
