/*
 * Simplest Terminal Tetris in C (Windows Only - Lab Project)
 *
 * Compile: gcc tetris.c -o tetris.exe
 * Run:     tetris.exe
 * Controls: A = Left, D = Right, W = Rotate, S = Drop, Q = Quit
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>    /* For _kbhit() and _getch() */
#include <windows.h>  /* For Sleep() */

#define WIDTH  10
#define HEIGHT 20

/* 1. The 7 classic Tetris shapes (4x4 grids) */
int SHAPES[7][4][4] = {
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, /* I */
    {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}, /* O */
    {{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}}, /* T */
    {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}}, /* S */
    {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}}, /* Z */
    {{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}}, /* J */
    {{0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0}}  /* L */
};

int board[HEIGHT][WIDTH] = {0};
int piece[4][4];
int px, py;          /* Current piece position */
int score = 0;

/* 2. Check if `p` can move by (dx, dy) without hitting walls or blocks */
int can_move(int dx, int dy, int p[4][4]) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (p[y][x] == 0) continue;
            int nx = px + x + dx;
            int ny = py + y + dy;
            if (nx < 0 || nx >= WIDTH || ny >= HEIGHT) return 0;
            if (ny >= 0 && board[ny][nx] == 1) return 0;
        }
    }
    return 1;
}

/* 3. Pick a random shape and place it at the top */
int spawn_piece(void) {
    int id = rand() % 7;
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            piece[y][x] = SHAPES[id][y][x];
    px = WIDTH / 2 - 2;
    py = 0;
    return can_move(0, 0, piece); /* Returns 0 if top is blocked (Game Over) */
}

/* 4. Rotate the current piece 90 degrees clockwise */
void rotate_piece(void) {
    int temp[4][4];
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            temp[x][3 - y] = piece[y][x];

    if (can_move(0, 0, temp)) {
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                piece[y][x] = temp[y][x];
    }
}

/* 5. Lock the piece onto the board and clear any full rows */
void lock_and_clear(void) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (piece[y][x] && py + y >= 0)
                board[py + y][px + x] = 1;

    for (int y = HEIGHT - 1; y >= 0; y--) {
        int full = 1;
        for (int x = 0; x < WIDTH; x++) {
            if (board[y][x] == 0) { full = 0; break; }
        }
        if (full) {
            for (int r = y; r > 0; r--)
                for (int x = 0; x < WIDTH; x++)
                    board[r][x] = board[r - 1][x];
            for (int x = 0; x < WIDTH; x++)
                board[0][x] = 0;
            score += 100;
            y++; /* Re-check the same row after shifting down */
        }
    }
}

/* 6. Draw the board, active piece, and score */
void draw(void) {
    system("cls");
    printf("TETRIS  |  Score: %d\n", score);
    printf("+--------------------+\n");
    for (int y = 0; y < HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < WIDTH; x++) {
            int filled = board[y][x];
            int bx = x - px, by = y - py;
            if (bx >= 0 && bx < 4 && by >= 0 && by < 4 && piece[by][bx])
                filled = 1;
            printf(filled ? "[]" : "  ");
        }
        printf("|\n");
    }
    printf("+--------------------+\n");
    printf("A/D: Move | W: Rotate | S: Drop | Q: Quit\n");
}

int main(void) {
    srand((unsigned)time(NULL));
    if (!spawn_piece()) return 0;

    int tick = 0, running = 1;
    while (running) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a' || key == 'A') { if (can_move(-1, 0, piece)) px--; }
            if (key == 'd' || key == 'D') { if (can_move(1, 0, piece))  px++; }
            if (key == 'w' || key == 'W') { rotate_piece(); }
            if (key == 's' || key == 'S') { if (can_move(0, 1, piece))  py++; }
            if (key == 'q' || key == 'Q') { running = 0; }
        }

        /* Gravity: move piece down every 10 loops (~500 ms) */
        if (++tick >= 10) {
            tick = 0;
            if (can_move(0, 1, piece)) {
                py++;
            } else {
                lock_and_clear();
                if (!spawn_piece()) running = 0;
            }
        }

        draw();
        Sleep(50);
    }

    printf("\nGame Over! Final Score: %d\n", score);
    return 0;
}
