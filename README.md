# Virtual Automotive ECU Simulation (Multi-Node SocketCAN)

## 🚗 Project Overview
This project simulates a complete vehicle powertrain and body network using the **Linux SocketCAN** stack. It demonstrates a **Centralized Architecture** where a Dashboard ECU processes data from multiple independent nodes simultaneously using a raw CAN protocol.

The system features **ID Arbitration** and **Message Filtering** to handle conflicting traffic from:
1.  **Powertrain Node (ID 0x123):** Broadcasts RPM/Temperature telemetry and handles Driver Throttle logic.
2.  **Body Control Node (ID 0x244):** Broadcasts Door Status (Open/Closed) and handles Locking logic.
3.  **Dashboard ECU (Receiver):** A central node that arbitrates between differing message IDs and visualizes the complete vehicle state in real-time.

## 🛠 Technical Highlights
* **Multi-Process Simulation:** Running 3 separate process contexts to mimic independent hardware ECUs on a single Linux machine.
* **CAN ID Arbitration:** The Receiver implements logic to distinguish Powertrain data (`0x123`) from Body data (`0x244`) on the same shared bus.
* **Bitwise Unpacking:** Manual deserialization of raw frames.
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
2. Compile All ECUs
Run these commands to build the three separate nodes.

Bash

g++ sender.cpp -o engine
g++ door_sender.cpp -o door
g++ receiver.cpp -o dashboard
3. Execution (Requires 3 Terminal Windows)
Open three separate terminals to simulate the distributed network.

Terminal 1: Dashboard (The Listener)

./receiver
Displays the real-time status of the car.

Terminal 2: Engine ECU

./sender
'a': Accelerate (+500 RPM)

'b': Brake (-500 RPM)

Terminal 3: Body ECU (Door)

./door_sender
'o': Open Door
'c': Close Door

📂 Project Structure
sender.cpp: Engine Control Unit logic (RPM Physics).

door_sender.cpp: Body Control Unit logic (Door State).

receiver.cpp: Central Dashboard logic (Data Visualization).
