#include "Board.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

Board::Board(int n) : size(n), grid(n, std::vector<char>(n, ' ')) {
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            grid[r][c] = ((r + c) % 2 == 0) ? 'B' : 'W';
        }
    }
}

void Board::print() {
    std::cout << "  ";
    for (int c = 0; c < size; ++c) std::cout << c << " ";
    std::cout << "\n";

    for (int r = 0; r < size; ++r) {
        std::cout << r << " ";
        for (int c = 0; c < size; ++c) {
            std::cout << grid[r][c] << " ";
        }
        std::cout << "\n";
    }
}

bool Board::hasMoves(char player) {
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (grid[r][c] == player) {
                int dr[4] = {0, 0, -2, 2};
                int dc[4] = {-2, 2, 0, 0};
                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];
                    int mr = r + dr[i]/2;
                    int mc = c + dc[i]/2;
                    if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
                        if (grid[nr][nc] == ' ' && grid[mr][mc] != player && grid[mr][mc] != ' ')
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::makeMove(int r1, int c1, int r2, int c2, char player) {
    if (r1 < 0 || r1 >= size || c1 < 0 || c1 >= size) return false;
    if (r2 < 0 || r2 >= size || c2 < 0 || c2 >= size) return false;
    if (grid[r1][c1] != player || grid[r2][c2] != ' ') return false;

    int dr = r2 - r1;
    int dc = c2 - c1;
    if (!((abs(dr) == 2 && dc == 0) || (abs(dc) == 2 && dr == 0))) return false;

    int mr = r1 + dr / 2;
    int mc = c1 + dc / 2;
    char mid = grid[mr][mc];
    if (mid == ' ' || mid == player) return false;

    grid[r2][c2] = player;
    grid[r1][c1] = ' ';
    grid[mr][mc] = ' ';
    return true;
}

void Board::removePiece(int row, int col) {
    if (row >= 0 && row < size && col >= 0 && col < size)
        grid[row][col] = ' ';
}


