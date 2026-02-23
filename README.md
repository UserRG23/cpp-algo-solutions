# C++ Algorithmic Suite: Structures & Optimization

### Overview

This repository contains a collection of advanced algorithmic solutions focused on state-space search, self-balancing data structures, and combinatorial optimization. These projects emphasize high-performance C++ implementation and rigorous memory management.

Key areas of focus:

* **State-Space Search:** Navigating complex multidimensional environments using BFS.
* **Self-Balancing Trees:** Implementing AVL trees for efficiency.
* **Combinatorial Optimization:** Solving profit-maximization problems via Dynamic Programming.

### Repository Structure

Each directory represents a core algorithmic challenge:

* **01_Hero_Pathfinder (BFS):** Shortest path through a monster-infested maze using state-space search.
* **02_Hobbit_Registry (AVL):** A Hobbit army manager using a self-balancing tree with range updates.
* **03_Vault_Heist (DP):** A knapsack-style optimizer for maximizing heist profits within a time limit.

---

### Getting Started

To compile and run these tasks, ensure you have `g++`, `make`, and `gdb` installed. Use the following commands from within any task directory:

| Command      | Action                                                           |
| ------------ | ---------------------------------------------------------------- |
| `make build` | Compiles the code into a binary located in the `bin/` directory. |
| `make run`   | Compiles and executes the program immediately.                   |
| `make debug` | Compiles with debug symbols and launches `gdb`.                  |

---

### Core Implementations

#### 1. Maze Pathfinder (BFS)

Solves the shortest path problem where the state is defined by `(Location, Current Inventory, Health)`. It accounts for turn-based combat and item-based stat scaling.

#### 2. Hobbit Army (AVL Tree)

Manages a sorted registry of entities. It implements specific rotations (LL, RR, LR, RL) to maintain balance and supports **range updates** (enchantment) across alphabetical bounds.

#### 3. Vault Heist (DP)

Calculates the optimal set of vaults to rob. It uses **Dynamic Programming** to handle the trade-off between vault value and the "transition time" required to move between locations.

---

### Tech Stack

* **Language:** C++ (C++20 standard)
* **Data Structures:** AVL Trees, Adjacency Lists, Priority Queues.
* **Paradigms:** Object-Oriented Programming, Functional decomposition, and RAII.
* **Tools:** Valgrind (for memory leak detection), G++ (GCC), GDB.
