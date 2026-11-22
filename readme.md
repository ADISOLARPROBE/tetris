# Terminal Tetris in C 

A simplified, single-file terminal Tetris game written in C for Windows, built as a C programming lab project. 149 lines of code.

## Team Members

| S.No | Name | Roll Number | 
| ----- | ----- | ----- | 
| 1 | Aditya Narayan Pattanaik | `AP26110100011` | 
| 2 | Daksh Vaishnav | `AP26110100022` | 
| 3 | Krish Shrivastava | `AP26110100020` | 

## Project Objectives

This project demonstrates core C programming concepts through an interactive console game:

* **2D & 3D Arrays**: Representing the `20x10` playfield (`board`) and the 7 Tetromino shapes (`SHAPES[7][4][4]`).

* **Matrix Transformation**: Rotating a `4x4` block 90° clockwise using index mapping.

* **Non-Blocking Console I/O**: Reading real-time keystrokes via `<conio.h>` without pausing program execution.

* **Game Loop & Timing**: Controlling frame rate and gravity ticks using `<windows.h>` (`Sleep()`).

* **Modular Functions**: Separating collision checking, spawning, rotation, line clearing, and rendering into clean, single-purpose functions.

## Features

* **Single-File Implementation**: Everything lives in `tetris.c` with no external header files or third-party libraries.

* **All 7 Classic Tetrominoes**: `I`, `O`, `T`, `S`, `Z`, `J`, and `L` stored as `4x4` binary matrices.

* **Non-Blocking Keyboard Input**: Uses `_kbhit()` and `_getch()` so pieces continue falling even when no key is pressed.

* **Collision Detection & Wall Bounds**: Prevents pieces from moving outside the `20x10` board or overlapping locked blocks.

* **Line Clearing & Scoring**: Automatically detects full rows, shifts upper rows downward, and awards `100` points per cleared line.

* **Game Over Detection**: Ends the game cleanly and displays the final score when a newly spawned piece is blocked at the top.

## Repository Structure

* `tetris.c` — Complete, self-contained C source code (149 lines, Windows-compatible).

* `README.md` — Project documentation, build instructions, and architecture overview.

## Controls

| Key | Action | 
| ----- | ----- | 
| `A` / `a` | Move Left | 
| `D` / `d` | Move Right | 
| `W` / `w` | Rotate 90° Clockwise | 
| `S` / `s` | Soft Drop (Move Down) | 
| `Q` / `q` | Quit Game | 

## Getting Started (Windows)

### Prerequisites

* **Operating System**: Windows 10 or Windows 11

* **Compiler**: **GCC** (via MinGW-w64 / TDM-GCC) or **Clang**

### Compile & Run

1. Clone or download this repository and open **Command Prompt** or **PowerShell** inside the project folder.

2. Compile `tetris.c`:

   ```
   gcc tetris.c -o tetris.exe
   
   ```

3. Run the executable:

   ```
   tetris.exe
   
   ```

### Troubleshooting

* **`'gcc' is not recognized`**: Ensure MinGW/GCC is installed and its `bin` directory (e.g., `C:\mingw64\bin`) is added to your Windows `PATH` environment variable.

* **Flickering Display**: Run `tetris.exe` in **Windows Terminal** or standard `cmd.exe` with at least 25 rows of vertical height so the entire board fits without scrolling.

## How the Game Works (Architecture & Logic)

1. **Board & Active Piece Separation**:

   * `board[20][10]` stores only settled blocks (`1` = filled, `0` = empty).

   * `piece[4][4]` holds the currently falling shape, with `(px, py)` tracking its top-left corner on `board`.

2. **Collision Detection (`can_move`)**:

   * Every movement or rotation tests target coordinates `(px + x + dx, py + y + dy)` against the left/right walls (`0..9`), the floor (`< 20`), and existing locked cells in `board`.

3. **90° Clockwise Rotation (`rotate_piece`)**:

   * Maps each cell `(y, x)` to `temp[x][3 - y]`. The rotated grid replaces `piece` only if `can_move(0, 0, temp)` succeeds.

4. **Game Loop & Gravity (`main`)**:

   * Each iteration runs every **50 ms** (`Sleep(50)`), checking `_kbhit()` for player input.

   * A `tick` counter triggers automatic downward movement every **10 iterations (\~500 ms)**.

5. **Locking & Line Clearing (`lock_and_clear`)**:

   * When a piece cannot move down, its cells are copied into `board`.

   * Rows are scanned from bottom (`y = 19`) to top (`y = 0`). Any full row shifts all rows above it down by one and adds `100` to `score` (re-checking the same row index via `y++` in case multiple rows are cleared together).

## Code Overview

| Component / Function | Description | 
| ----- | ----- | 
| `board[20][10]` & `SHAPES[7][4][4]` | Stores locked blocks on the `20x10` grid and the 7 Tetromino templates. | 
| `can_move(dx, dy, p)` | Checks if piece `p` can move by `(dx, dy)` without hitting walls, the floor, or locked blocks. | 
| `spawn_piece()` | Picks a random shape (`rand() % 7`), places it at the top-center (`px = 3, py = 0`), and checks for Game Over. | 
| `rotate_piece()` | Rotates the active piece 90° clockwise into a temporary `4x4` grid and applies it only if valid. | 
| `lock_and_clear()` | Locks the landed piece onto `board`, clears full rows, shifts upper rows down, and updates `score`. | 
| `draw()` | Clears the console (`system("cls")`) and renders the board, active piece, score, and controls. | 
| `main()` | Runs the 50 ms game loop handling keyboard input, gravity ticks (\~500 ms), and screen updates. | 

## Sample Terminal Output

```
TETRIS  |  Score: 200
+--------------------+
|                    |
|        [][]        |
|        []          |
|        []          |
|                    |
|  [][][][]    [][]  |
|  [][]  [][][][][]  |
+--------------------+
A/D: Move | W: Rotate | S: Drop | Q: Quit

```

## Possible Future Improvements

* Display a **Next Piece** preview box beside the board.

* Increase gravity speed automatically as the player's score rises (level progression).

* Add colored Tetrominoes using Windows Console API attributes (`SetConsoleTextAttribute`).

* Save and load the all-time **High Score** using a local text file (`highscore.txt`).

## Acknowledgments

* Simplified from [svedev0/tetris-c](https://github.com/svedev0/tetris-c) for educational and lab demonstration purposes.
