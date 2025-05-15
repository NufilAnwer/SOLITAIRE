# 🃏 Solitaire Game in C++ (Data Structures + OOP)

Welcome to the **Solitaire (Klondike)** card game built entirely in **C++** using **custom data structures** and **object-oriented programming principles**. This project simulates the popular solitaire gameplay logic with a text-based interface, card movements, and undo capabilities — all without using STL containers like `vector`, `stack`, or `map`.

---

## 📦 Features

- Full **Klondike Solitaire rules** implemented
- Move cards between:
  - Tableau columns
  - Foundations (Aces piles)
  - Wastepile and Stock
- **Draw cards** from stock
- **Undo last move**
- **Auto-reveal** next card after a move
- **Win detection**
- Command-line interface to play
- Optimized with **custom-built data structures** for learning purposes

---

## 💡 Object-Oriented Design & Concepts Used

| OOP Concept       | Application                                                                 |
|-------------------|------------------------------------------------------------------------------|
| **Encapsulation** | Private members with public access methods in all classes                   |
| **Abstraction**   | Classes like `Card`, `Stack`, `List`, and `Game` abstract implementation    |
| **Inheritance**   | Shared behavior between different pile types (e.g., foundation, tableau)    |
| **Polymorphism**  | Command processing, undo system                                             |
| **Association**   | Cards associated with lists, piles, and players                             |
| **Aggregation**   | Game aggregates piles that can exist independently                          |
| **Composition**   | Each `Game` object *owns* its piles and cards — destroyed when the game ends|

---

## 🧰 Data Structures Implemented

| Structure             | Purpose                             |
|------------------------|--------------------------------------|
| **Stack (custom)**     | For foundations, wastepile, undo     |
| **Doubly Linked List** | For tableau columns                  |
| **Iterator Class**     | Traverse and manipulate linked lists |
| **Command Stack**      | Store and reverse previous moves     |

✅ **Note:** No STL containers are used. All structures are implemented manually for educational clarity.

---

## 🎮 How to Play

### Commands
```bash
draw               # Draw card from stock to waste
move T1 F1         # Move card from Tableau 1 to Foundation 1
move T3 T5         # Move cards from Tableau 3 to Tableau 5
move W T2          # Move top of Wastepile to Tableau 2
undo               # Undo last valid move
restart            # Restart the game
exit               # Exit the game
