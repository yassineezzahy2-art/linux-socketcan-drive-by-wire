# Linux SocketCAN Drive-by-Wire Simulator (v3.0 Bi-Directional)

## 🚗 Project Overview
This project simulates a **Safety-Critical Automotive Control System** using Linux SocketCAN. 
It demonstrates a fully asynchronous **Transceiver Architecture**, capable of broadcasting telemetry (RPM/Door) while simultaneously listening for high-priority network commands (Remote Kill Switch).

## ⚡ Key Features (v3.0)
* **Bi-Directional Communication:** The Engine Node is no longer just a sender; it is a full transceiver using `O_NONBLOCK` sockets.
* **Emergency Kill Switch:** The system listens for CAN ID `0x666`. Upon reception, it overrides user input and cuts engine power (RPM = 0) immediately.
* **Real-Time Physics:** 100Hz control loop simulating engine inertia and friction.
* **State Machine Logic:** Debounced door control logic to prevent signal flickering.

## 🛠️ Tech Stack
* **Language:** C++
* **Protocol:** CAN (Raw Sockets)
* **Kernel Features:** `fcntl` (Non-blocking I/O), `termios` (Terminal Control).
* **Architecture:** Publisher/Subscriber + Command Pattern.

## 🎮 Controls
* **[W]** - Accelerate
* **[S]** - Brake
* **[D]** - Toggle Door
* **[Q]** - Quit

## 🚨 How to Test Safety Features
1. Run the engine: `./engine_node`
2. Rev to 3000 RPM.
3. Open a separate terminal and inject a "Kill Command":
   ```bash
   cansend vcan0 666#
Observe immediate engine shutdown.
