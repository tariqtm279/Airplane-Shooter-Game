# Airplane Flight Shooter

> ✈️ A simple 2D airplane shooter game developed in **C++** as an academic project.

![Language](https://img.shields.io/badge/Language-C++-00599C?style=flat-square\&logo=cplusplus\&logoColor=white)
![Compiler](https://img.shields.io/badge/Compiler-GCC-F05032?style=flat-square\&logo=gnu\&logoColor=white)
![Graphics](https://img.shields.io/badge/Graphics-OpenGL%20%2F%20GLUT-success?style=flat-square)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=flat-square)

---

## 📖 Overview

**Airplane Flight Shooter** is a simple 2D shooting game developed in **C++**. The project demonstrates the implementation of fundamental game programming concepts, including object management, game state handling, collision detection, and graphical rendering using OpenGL/GLUT.

The game is structured using a modular architecture, separating graphics, game logic, entities, and utility functions into different source files.

---

## ✨ Features

* 2D Airplane Shooter Gameplay
* Player Movement
* Enemy Objects
* Collision Detection
* Score Management
* Game State Management
* Modular Source Code
* OpenGL Rendering

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

| Technology | Description                          |
| ---------- | ------------------------------------ |
| C++        | Primary programming language         |
| OpenGL     | Graphics rendering                   |
| GLUT       | Window management and input handling |
| Dev-C++    | Development environment              |

---

## 🚀 Getting Started

Clone the repository.

```bash
git clone https://github.com/<username>/AirplaneFlightShooter.git
```

Navigate to the project directory.

```bash
cd AirplaneFlightShooter
```

Compile the project using your preferred C++ compiler with OpenGL and GLUT installed.

Example:

```bash
g++ main.cpp src/*.cpp -Iinclude -lglut -lGL -lGLU -o AirplaneFlightShooter
```

Run the executable.

```bash
./AirplaneFlightShooter
```

> **Note:** Compilation commands may differ depending on your operating system and compiler.

---

## 🎮 Gameplay

The player controls an airplane and must avoid or destroy incoming obstacles and enemies while achieving the highest possible score.

---

## 🏗️ Project Architecture

The project is organized into several modules:

| Module      | Responsibility                    |
| ----------- | --------------------------------- |
| `Graphics`  | Rendering objects and game scenes |
| `GameState` | Managing gameplay states          |
| `Entities`  | Defining in-game objects          |
| `City`      | Environment rendering             |
| `MathUtils` | Mathematical helper functions     |
| `Config`    | Global configuration values       |

---

## 📚 Learning Outcomes

This project demonstrates practical implementation of:

* Object-Oriented Programming (OOP)
* Modular Programming
* OpenGL Graphics Programming
* Game Loop Architecture
* Collision Detection
* State Management
* Event Handling

---

## 🤝 Collaborators

* Arsy Thariq Munawar
* Yuma Hazza Yuditama
* Johan Reinhart Calvin
* Shalom Kurniawan

---

## 📄 License

This repository is intended for educational and academic purposes. The source code is provided for learning, reference, and documentation of coursework.
