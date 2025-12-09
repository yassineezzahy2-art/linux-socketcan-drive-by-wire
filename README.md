# Virtual Automotive ECU Simulation (Multi-Node SocketCAN)

## 🚗 Project Overview
This project simulates a complete vehicle powertrain and body network using the **Linux SocketCAN** stack. It demonstrates a **Centralized Architecture** where a Dashboard ECU processes data from multiple independent nodes simultaneously using a raw CAN protocol.

The system features **Real-Time Communication** and **Message Filtering** to handle conflicting traffic from:
1.  **Powertrain Node (ID 0x123):** Broadcasts RPM/Temperature telemetry at **10Hz** and handles Driver Throttle logic.
2.  **Body Control Node (ID 0x244):** Broadcasts Door Status (Open/Closed) and handles Locking logic.
3.  **Dashboard ECU (Receiver):** A central node that renders a live, flicker-free instrument cluster using ANSI escape codes.

## 🛠 Technical Highlights
* **Real-Time Communication:** The Engine ECU broadcasts telemetry at **10Hz** (every 100ms) regardless of user input, simulating a real vehicle bus.
* **Non-Blocking IO:** Implements `termios` and `fcntl` logic to capture keyboard input without pausing the main execution loop.
* **Live Dashboard UI:** Uses ANSI Escape Codes (`\033[2J`) to render a flicker-free, updating instrument cluster in the terminal instead of a scrolling log.
* **CAN ID Arbitration:** The Receiver implements logic to distinguish Powertrain data (`0x123`) from Body data (`0x244`) on the same shared bus.
* **Safety Logic:** Software-defined guard rails (Integer Underflow Protection, Rev Limiter).

## ⚙️ Prerequisites
* **OS:** Linux (Ubuntu/Debian)
* **Kernel:** `vcan` module enabled.
* **Tools:** `can-utils` (Optional, for debugging)

## 💻 How to Run

### 1. Setup Virtual Network
The project requires a virtual interface to simulate the physical bus.
```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
### 2. Compile All ECUs
Run these commands to build the three separate nodes.

Bash

g++ sender.cpp -o sender
g++ receiver.cpp -o receiver
### 3. Execution (Requires 3 Terminal Windows)
Open three separate terminals to simulate the distributed network.

Terminal 1: Dashboard (The Listener)

Bash

./receiver
Displays the real-time Live UI.

Terminal 2: Engine ECU (Real-Time)

Bash

./sender
'w': Rev Engine (Hold to accelerate)

's': Brake

'q': Quit
