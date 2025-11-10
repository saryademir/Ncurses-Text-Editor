# Ncurses-Based Text Editor

This project was developed as part of the **CSE232** course at **Yeditepe University**.  
It is a simple **terminal-based text editor** written in **C** using the **ncurses** library.

---

##  Overview

The program allows users to open, edit, insert, and delete lines in a text file directly from the terminal.  
It uses a **custom array-based linked list** structure to manage text lines efficiently.

---

## Features

- **Open / Edit** an existing text file  
- **Insert** new lines  
- **Delete** lines  
- **Save** changes to file  
- **Exit** safely from the ncurses interface  
- Displays a **menu bar** for available key commands

---

## ⌨️ Controls

| Key | Action |
|-----|--------|
| `W` or `w` | Move cursor up |
| `Z` or `z` | Move cursor down |
| `I` or `i` | Insert a new line |
| `D` or `d` | Delete the current line |
| `S` or `s` | Save changes to file |
| `X` or `x` | Exit the program |

---

## Implementation Details

- **Language:** C  
- **Libraries:** `<stdio.h>`, `<string.h>`, `<stdlib.h>`, `<curses.h>`  
- **Data Structure:** Array-based linked list (`struct node { char statement[40]; int next; }`)  
- **Editor Commands:** Implemented through ncurses key input loop

