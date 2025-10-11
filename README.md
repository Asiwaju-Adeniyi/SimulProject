# N-Body Gravitational Simulation (Earth–Moon–Sun Model)

A C++ and OpenGL–based **N-body simulation** using the **Velocity Verlet integration** method.  
This project is part of a collaborative research effort inspired by the work of **Dr. John L. Gustafson**, a pioneer in **High-Performance Computing (HPC)** and numerical accuracy.

---

## About Dr. John L. Gustafson

**Dr. John L. Gustafson** is an American computer scientist known for groundbreaking contributions in **parallel computing**, **performance modeling**, and **computer arithmetic**.

### Key Achievements:
- **Invented Gustafson’s Law** — a fundamental principle in parallel computing that redefined scalability analysis.
- **Introduced the first commercial computer cluster**, leading to today’s parallel supercomputers.
- **Winner of the inaugural Gordon Bell Award (1988)** for exceptional achievement in parallel processing.
- **Developed QUIPS (Quality Improvement Per Second)** — a metric for measuring real-world computing performance.
- **Created the Posit and Unum number systems**, alternatives to IEEE floating-point arithmetic, offering superior accuracy and energy efficiency.
- **Authored *The End of Error: Unum Computing* (2015)**, exploring the next generation of numerical computation.
- **Recipient of the IEEE Computer Society Golden Core Award and multiple R&D 100 Awards.**
- **Holds degrees from Caltech** (B.S., Applied Mathematics) **and Iowa State University** (M.S., Ph.D., Applied Mathematics).

Dr. Gustafson’s ongoing research explores **b-posits** — a bounded variant of the posit number system — and their potential for improving numerical stability in large-scale scientific computations.

---

## About This Project

This repository implements a **three-body gravitational model** (Sun–Earth–Moon) to study **numerical precision and dynamic range** in floating-point systems — particularly as a testbed for **b-posit arithmetic**.

### Research Goal

The goal is to evaluate how different arithmetic representations (IEEE floats vs. b-posits) handle **extreme scale variation** in gravitational problems:

\[
F = G \frac{m_1 m_2}{r^2}
\]

This simulation is especially suitable because:
- The gravitational constant **G** is extremely small (~6.674×10⁻¹¹),
- Masses (**m₁, m₂**) are huge (10²⁴–10³⁰ kg),
- Distances (**r**) are astronomical (10⁶–10¹¹ m).

Such combinations produce **numerical edge cases** where rounding, overflow, or loss of significance can distort results — making it an excellent benchmark for new number systems like **b-posits**.

---

## Technical Overview

### Languages & Libraries
- **C++17** for simulation logic  
- **GLFW + GLAD + OpenGL** for visualization  
- **GLM (OpenGL Mathematics)** for vector math  
- **CMake** for cross-platform builds

### Core Components
| Module | Purpose |
|---------|----------|
| `physics/solver.*` | Implements Velocity Verlet integration for motion |
| `physics/body.*` | Defines celestial body properties (mass, position, velocity) |
| `render/renderer.*` | Handles OpenGL rendering of trajectories |
| `utils/constants.*` | Physical constants (G, masses, orbital radii) |

---

## Features

- Real-time simulation of Sun, Earth, and Moon  
- Accurate gravitational dynamics using the **Velocity Verlet** method  
- OpenGL rendering with colored trails for each body  
- Modular architecture for future extension (e.g., b-posit arithmetic)  
- Clean CMake build system — runs on macOS, Linux, and Windows  

---

## Next Steps

- Implement **b-posit arithmetic** to replace IEEE floating-point operations.  
- Run comparative accuracy tests between float, double, and b-posit.  
- Visualize orbital drift or instability due to limited precision.  
- Extend to N > 3 bodies for generalized dynamic range experiments.

---

## Build Instructions

### Prerequisites
- CMake ≥ 3.10  
- GLFW  
- GLM  
- OpenGL drivers

### Build & Run
```bash
git clone https://github.com/Asiwaju-Adeniyi/Fogo-Folu.git
cd Physics-Simulation-Project
cmake -S . -B build
cmake --build build
./build/AsiwajuAdeniyi
