#pragma once
#include <vector>
#include <string>

class Board {
public:
    Board(int n);
    void print();
    bool hasMoves(char player);
    bool makeMove(int r1, int c1, int r2, int c2, char player);
    void saveToFile(const std::string &filename);
    void loadFromFile(const std::string &filename);
    void removePiece(int row, int col);

private:
    int size;
    std::vector<std::vector<char>> grid;
};
