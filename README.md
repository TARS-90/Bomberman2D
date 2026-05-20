# 💣 Bomberman 2D

A competitive, top-down arcade game designed for multiplayer gameplay.

---

## 📝 Game Description

### Overview
**Bomberman 2D** is an arcade game for **2 to 4 players**. Players compete against each other on a closed grid map, placing time-delay bombs. The core gameplay revolves around tactical wall destruction, gathering power-ups, and trapping or eliminating opponents.

* **Genre:** Competitive Arcade
* **Focus:** Simple mechanics, tactical and logical elements

### Gameplay Flow
1. Each player starts the match in a different corner of the map.
2. Players move around the grid and drop explosives, attempting to trap their opponents.
3. The match continues until **only one surviving player** remains on the board.

---

## 🎮 Gameplay & Mechanics

### Objective
The main goal is to eliminate all other players by detonating bombs near them. Each explosion deducts one life from an affected player. Losing all lives results in immediate elimination from the match.

### Controls
* `W`, `A`, `S`, `D` – Character movement
* `Spacebar` – Place a bomb

### ⏱️ Sudden Death Mechanic
If a match goes on for too long and players avoid engagement, a sudden death mechanic is triggered. **Fire** begins to spawn in each corner of the map, gradually spreading towards the center and shrinking the safe playable area.

> ⚠️ **No Draws:** The game always declares a single winner. If the same bomb eliminates the last remaining players simultaneously, the outcome is determined by the order in which the server processes the event (critical section).

---

## ⚙️ Architecture (Client-Server)

The game is built on a **client-server** architecture to ensure fair gameplay and prevent cheating:

* **Server as the Source of Truth:** The entire game state (players' health, object positions, inventory, bomb locations) is authoritatively stored and processed on the server.
* **Movement Validation:** Every player (client) action is sent as a request to the server. The server verifies whether the movement or bomb placement is valid.
* **Synchronization:** Once a state change is approved, the server broadcasts the updated game state to all connected clients during the next update cycle.

---

## 🛠️ Tech Stack

This project is implemented utilizing **strictly open-source technologies**:

| Component | Technology / Tool | Purpose |
| :--- | :--- | :--- |
| **Server** | C Language | Game logic, state management, and synchronization |
| **Client** | C Language + SDL | Graphics rendering, window management, and input handling |
| **Networking** | UNIX Sockets API | Low-level network communication between client and server |
| **Build System** | CMake | Project structure and compilation management |
| **Graphics** | GIMP | Creating and editing textures or sprites |
