# Virtual CAN Bus Communication System

## Project Overview
This project implements a **Vehicle Network Simulation** using C++ and the **Linux SocketCAN** API.
It demonstrates low-level understanding of automotive protocols, real-time data exchange, and embedded systems architecture.

## Architecture
- **Tech Stack:** C++14, Linux Kernel, SocketCAN.
- **Protocol:** Raw CAN Frames (Controller Area Network).
- **Topology:**
  - `can_sender`: Simulates an ECU broadcasting sensor data (ID: 0x555).
  - `can_receiver`: Simulates a Dashboard/Gateway listening for critical frames in a blocking loop.

## How to Run
1. **Setup Virtual Interface:**
   ```bash
   sudo modprobe vcan
   sudo ip link add dev vcan0 type vcan
   sudo ip link set up vcan0
2. Compile:

3. Execute:

Terminal 1: ./can_receiver

Terminal 2: ./can_sender
