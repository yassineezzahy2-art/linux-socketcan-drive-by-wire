# Linux SocketCAN Drive-by-Wire Simulator

## 🚗 Project Overview
This project implements a **Drive-by-Wire (DBW) simulation** using the Linux SocketCAN stack. It mimics the behavior of a modern automotive throttle control system by establishing communication between two virtual ECUs.

The system features an **Interactive Control Loop** where user input serves as the pedal sensor, dynamically adjusting engine parameters in real-time.

## ⚙️ System Architecture
The project consists of two isolated nodes communicating over a virtual CAN bus (`vcan0`):

### 1. Engine Control Unit (Sender)
* **Role:** Acts as the main powertrain controller.
* **Input Logic:** Captures keyboard interrupts to simulate pedal actuation:
    * **'W' Key (Throttle):** Simulates acceleration (Increments RPM).
    * **'B' Key (Brake):** Simulates braking (Decrements RPM).
* **Protocol:** Packs the 16-bit RPM integer into a standard CAN Frame (ID `0x123`) using bitwise byte separation (High Byte / Low Byte).

### 2. Instrument Cluster (Receiver)
* **Role:** Acts as the dashboard display unit.
* **Logic:** Listens to the CAN network for ID `0x123`.
* **Decoding:** Reconstructs the raw byte stream back into human-readable integer values.
* **Safety Logic:** Monitors limits and triggers warnings if RPM exceeds safe thresholds (e.g., Redline Warning).

## 🛠️ Technology Stack
* **Language:** C++ (Low-level memory management)
* **API:** Linux SocketCAN (`<linux/can.h>`, `<sys/socket.h>`)
* **Network Interface:** Virtual CAN (`vcan`)
* **Concepts:** Bitwise operations, Structs, Network Byte Order, Real-time Simulation.

## 🚀 How to Run

1.  **Initialize Virtual CAN:**
    ```bash
    sudo modprobe vcan
    sudo ip link add dev vcan0 type vcan
    sudo ip link set up vcan0
    ```

2.  **Compile the Nodes:**
    ```bash
    g++ sender.cpp -o engine_node
    g++ receiver.cpp -o cluster_node
    ```

3.  **Execute:**
    * Run `./cluster_node` in Terminal 1.
    * Run `./engine_node` in Terminal 2.
    * *Press 'W' to accelerate and 'B' to brake.*

---
*Developed as part of an Embedded Automotive Engineering portfolio.*
