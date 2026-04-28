# 2D Animated Solar System

**A Computer Graphics course project created in C++ using OpenGL and GLUT.**
This project demonstrates a 2D animated solar system, with dynamic motion of planets, moons, and a comet.

---

## Features

* **DDA Line Algorithm**: Used to draw lines between objects.
* **Midpoint Circle Algorithm**: Used for drawing circles, representing planets and orbits.
* **3D Transformations**:

  * **Translation**: To move objects (planets and comet).
  * **Rotation**: To make planets and moons revolve around their orbits.
  * **Scaling**: To animate the Sun with pulsing size.
* **Animated Objects**:

  * Planets (Venus, Earth, Mars, Jupiter, Saturn)
  * Moons (Earth's Moon)
  * Comet
* **Saturn's Rings**: Represented by dynamic rings around Saturn.
* **Twinkling Stars**: Background stars that flicker and twinkle.
* **Keyboard Controls**: Easy user interaction to control speed and pause.
* **Smooth Animation**: With adjustable speed using arrow keys and pause functionality.

---

## Algorithms Used

1. **DDA (Digital Differential Analyzer) Line Algorithm**:

   * Efficiently draws lines by incrementing coordinates.
2. **Midpoint Circle Algorithm**:

   * Used to generate circles by calculating pixel positions using the midpoint decision rule.

---

## Transformations Used

1. **Translation**:

   * Moves objects (planets, moons, and comet) in space relative to the center (the Sun).
2. **Rotation**:

   * Makes the planets rotate around the Sun and moons around their respective planets.
3. **Scaling**:

   * Dynamically changes the size of the Sun, making it pulse.

---

## Animated Objects

* **Venus**: Rotates around the Sun in its orbit.
* **Earth**: Rotates around the Sun, and the Moon rotates around Earth.
* **Mars**: Rotates around the Sun.
* **Jupiter**: The largest planet, rotating around the Sun.
* **Saturn**: Rotates around the Sun with a ring system.
* **Comet**: Animates across the screen with a tail.
* **Stars**: Background stars that twinkle at random intervals.

---

## Controls

* **UP Arrow**: Increases animation speed.
* **DOWN Arrow**: Decreases animation speed.
* **SPACE**: Pauses and resumes the animation.

---

## Requirements

* **C++** (for compilation)
* **OpenGL**: For rendering the graphical objects.
* **GLUT**: Used for window creation and user interaction.
* **Timer Functionality**: Used for smooth animation and updates.

---

## Installation

**To install dependencies:**

1. **Ensure OpenGL and GLUT libraries are installed** on your system.
2. **Clone the repository**:

   ```bash
   git clone https://github.com/yourusername/2d-animated-solar-system.git
   cd 2d-animated-solar-system
   ```
3. **Compile the C++ code** (ensure you have a C++ compiler like `g++` installed):

   ```bash
   g++ -o solar_system main.cpp -lGL -lGLU -lglut
   ```
4. **Run the program**:

   ```bash
   ./solar_system
   ```

---

## Acknowledgements

* **GLUT/OpenGL**: For rendering and window management.
* **OpenGL and Computer Graphics Textbooks**: Used as references for implementing algorithms and transformations.

## License
This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.

---