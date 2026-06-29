# ✈️ Airplane Flight Shooter

> A 2D airplane shooting game developed in **C++** using **OpenGL** and **GLUT**, featuring real-time combat, collision detection, and modular game architecture.

![Language](https://img.shields.io/badge/Language-C++-00599C?style=flat-square\&logo=cplusplus\&logoColor=white)
![Graphics](https://img.shields.io/badge/Graphics-OpenGL-success?style=flat-square)
![Library](https://img.shields.io/badge/Library-GLUT-orange?style=flat-square)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=flat-square)

---

## 📖 Overview

**Airplane Flight Shooter** is a 2D arcade-style shooting game developed in **C++** with **OpenGL** and **GLUT**. The game challenges players to control a fighter aircraft, defeat incoming enemies, avoid collisions, and survive as long as possible while achieving the highest score.

The project was designed to demonstrate fundamental concepts of computer graphics and game development, including rendering, object-oriented programming, collision detection, game state management, and real-time user interaction.

---

## ✨ Features

* ✈️ Smooth airplane movement
* 🎯 Real-time shooting mechanics
* 👾 Enemy spawning system
* 💥 Bullet and enemy collision detection
* ❤️ Player health/life system
* ⭐ Score tracking
* 🏙️ Animated scrolling background
* 🎮 Responsive keyboard controls
* 🧠 Game state management (Menu, Playing, Game Over)
* 🧩 Modular C++ project structure
* 🖥️ OpenGL-based rendering

---

## 🎮 Controls

| Key           | Action     |
| ------------- | ---------- |
| **W** / **↑** | Move Up    |
| **S** / **↓** | Move Down  |
| **A** / **←** | Move Left  |
| **D** / **→** | Move Right |
| **Space**     | Shoot      |
| **Esc**       | Exit Game  |

> **Note:** Update this table if your project uses different key bindings.

---

## 🎯 Gameplay

Players control a fighter aircraft and must eliminate incoming enemies while avoiding collisions. Every defeated enemy increases the player's score. The game ends when the player's health reaches zero or another game-over condition is met.

The main objective is to survive for as long as possible and achieve the highest score.

---

## 📂 Project Structure

```text
AirplaneFlightShooter/
├── include/
│   ├── City.h
│   ├── Config.h
│   ├── Entities.h
│   ├── GameState.h
│   ├── Graphics.h
│   └── MathUtils.h
│
├── src/
│   ├── City.cpp
│   ├── GameState.cpp
│   └── Graphics.cpp
│
├── main.cpp
├── Makefile.win
└── AirFlightShooter.dev
```

---

## 🛠️ Technologies

| Technology  | Purpose                            |
| ----------- | ---------------------------------- |
| **C++**     | Core programming language          |
| **OpenGL**  | Graphics rendering                 |
| **GLUT**    | Window creation and input handling |
| **Dev-C++** | Development environment            |

---

## 🚀 Getting Started

### Clone the Repository

```bash
git clone https://github.com/<username>/AirplaneFlightShooter.git
```

### Navigate to the Project

```bash
cd AirplaneFlightShooter
```

### Compile

Example using **G++**:

```bash
g++ main.cpp src/*.cpp -Iinclude -lglut -lGL -lGLU -o AirplaneFlightShooter
```

### Run

```bash
./AirplaneFlightShooter
```

> **Note:** Compilation commands may vary depending on your operating system and compiler.

---

## 🏗️ Project Architecture

| Module        | Description                                              |
| ------------- | -------------------------------------------------------- |
| **Graphics**  | Handles rendering of game objects and scenes             |
| **GameState** | Manages game flow and state transitions                  |
| **Entities**  | Defines player, enemies, bullets, and other game objects |
| **City**      | Renders the background environment                       |
| **MathUtils** | Provides mathematical helper functions                   |
| **Config**    | Stores global constants and configuration values         |

---

## 📚 Learning Outcomes

This project demonstrates practical implementation of:

* Object-Oriented Programming (OOP)
* Modular Programming
* Computer Graphics using OpenGL
* Real-Time Rendering
* Collision Detection
* Keyboard Input Handling
* Game Loop Architecture
* State Management

---

## 📸 Gameplay Preview

> Add screenshots or GIFs of your gameplay here.

```text
assets/
├── gameplay.png
├── menu.png
└── gameplay.gif
```

Example:

```md
<p align="center">
  <img src="assets/gameplay.gif" width="700">
</p>
```

---

## 🤝 Collaborators

* Arsy Thariq Munawar
* Yuma Hazza Yuditama
* Shalom Kurniawan

---

## 📄 License

This project is intended for **educational and academic purposes**. The source code is provided for learning, experimentation, and documentation of coursework.

---

<div align="center">

**Developed with C++, OpenGL, and GLUT**

⭐ Feel free to explore the project and use it as a learning reference.

</div>
