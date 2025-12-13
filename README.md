# Embedded Linux Automotive Portfolio (C++ / SocketCAN)

## 📌 Project Overview
This repository demonstrates a complete **Automotive Software Stack** implemented in C++ using Linux SocketCAN. It is divided into two major modules:
1.  **Application Layer:** A Real-Time Drive-by-Wire Simulator (Engine & Dashboard).
2.  **Transport Layer:** A custom implementation of the **ISO-TP (ISO 15765-2)** protocol for handling large diagnostic messages.

---

## 🚗 Module A: Drive-by-Wire Simulator
A real-time simulation of an Engine Control Unit (ECU) and Instrument Cluster.

### Key Features
* **Asynchronous Transceiver:** Engine node uses `O_NONBLOCK` sockets to send telemetry while simultaneously listening for commands.
* **Safety Critical Logic:** Implements a "Remote Kill Switch" (ID `0x666`) that overrides user input and cuts power immediately.
* **Physics Simulation:** Calculates RPM decay (friction) and inertia in a 100Hz control loop.
* **State Machine:** Debounced door control logic.

### Source Files
* `sender.cpp` - Engine ECU (Physics & Transceiver).
* `receiver.cpp` - Digital Instrument Cluster (UI & Visualization).

---

## 📦 Module B: Custom ISO-TP Stack (ISO 15765-2)
A from-scratch implementation of the Transport Layer logic used in UDS Diagnostics. This stack handles the segmentation of large data (>8 bytes) into standard CAN frames.

### Key Features
* **Segmentation ("The Zipper"):** Automatically splits large messages (e.g., VIN, Firmware chunks) into:
    * **Single Frames (SF)** for small messages.
    * **First Frames (FF)** & **Consecutive Frames (CF)** for multi-packet messages.
* **Reassembly ("The Glue"):** Receiver buffers incoming frames, handles flow state, and reconstructs the full payload.
* **Bitwise Logic:** Manual handling of Protocol Control Information (PCI) bytes, Nibble swapping, and Padding (`0xAA`).

### Source Files
* `isotp_test.cpp` - The Sender logic (Segmentation & Sequence Numbers).
* `isotp_receiver.cpp` - The Receiver logic (Buffer Management & State Machine).

---

## 🛠️ Tech Stack
* **Language:** C++ (System Level)
* **Protocols:** CAN (Raw Sockets), ISO-TP (ISO 15765-2)
* **OS APIs:** `<sys/socket.h>`, `<linux/can.h>`, `fcntl` (Non-blocking I/O)
* **Tools:** `can-utils` (candump, cansend), Virtual CAN (`vcan0`)

## 🚀 How to Run

### 1. Setup Virtual CAN
```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
2. Run ISO-TP Demo
Terminal 1 (Receiver):

Bash

g++ isotp_receiver.cpp -o rx_node
./rx_node
Terminal 2 (Sender):

Bash

g++ isotp_test.cpp -o tx_node
./tx_node
Observe the VIN (WBAM33...) being segmented and reconstructed.

Developed by Yassine. Focused on Autosar, UDS, and Real-Time Embedded Systems.
