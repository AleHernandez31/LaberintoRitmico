# 🎵 Laberinto Rítmico
_Rhythm Maze Adventure – C++ & SFML 2.6_

[🎮 Play on itch.io](https://alehernandez31.itch.io/laberinto-ritmico)

**Laberinto Rítmico** is a rhythm-based maze game developed in **C++** using **SFML 2.6**.  
Your goal is to move across a grid while following the rhythm of the song, landing on the correct node at the right moment and avoiding trap nodes.

---

## 🎮 Gameplay

- Notes are loaded from CSV and synchronized with the music.
- Each note generates:
  - A **correct node** (target)
  - One or more **trap nodes**
- Dash using **W / A / S / D** to move from cell to cell.
- Land on the correct node **on time** to score:
  - **Perfect**
  - **Good**
  - **Bad**
- Missing notes or stepping on traps reduces your life.
- Life is linked to music volume: lower life = lower volume.
- When life reaches 0 or the song ends, the level finishes.

---

## ✨ Features

- **Responsive dash movement** with squash & stretch animation.
- **Tempo halo** indicating when to reach the target.
- **Particle effects** for Perfect/Good hits.
- **Dynamic map rendering** that follows the player.
- **Adaptive difficulty** (fewer traps after repeated fails).
- **Binary high-score system** saving the Top 10 scores.
- **Multiple levels**, each with their own CSV chart and song.

---

## 🧩 Project Structure

- `Juego` – Main game controller and state machine.
- `Menu / SubMenu` – Main menu, level selection, pause, and end-screen UI.
- `Player` – Movement, dash logic, animations and life.
- `GestorNodos` – Rhythm logic: correct node, traps, timing windows, hit detection.
- `Cancion` – CSV song loader and note iterator.
- `Map` – Grid rendering (roads, blocks, intersections).
- `ParticleSystemNodo` – Simple pixel-based particle bursts.
- `Scoring` – Accuracy and score calculation.
- `ScoringArchivo` – Binary file persistence (`scoring.dat`).

---

## ⌨️ Controls

**In-game**
- `W / A / S / D` – Dash
- `ESC` – Pause

**Menus**
- `W / S` – Navigate  
- `ENTER` – Select  
- `ESC` – Back

---

## 🛠 Requirements

- C++17 or newer  
- SFML 2.6+  
- Assets included in `/assets/`

To compile, link SFML modules:

- graphics
- window
- system
- audio


---

## 📈 High Scores

The game saves the Top 10 scores in a binary file (`scoring.dat`).  
Scores include:
- Total points  
- Average timing accuracy  
- Hit statistics (Perfect / Good / Bad)

---

## 📜 License

This is an academic project created for educational purposes.

---

## 👥 Developers

- **Alejandro Hernández**
- **Ricardo Gieco**
- **Lucas Battiato**

---

## 👥 Special Thanks

- **Brian Lara**
