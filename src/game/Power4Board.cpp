//
// Created by bananasmoothii on 21/07/2023.
//

#include <stdexcept>
#include <iostream>
#include "Power4Board.hpp"

Power4Board::Power4Board(int width, int height) : width(width), height(height) {
    board = std::vector<std::vector<short>>(height, std::vector<short>(width, 0));
}

int Power4Board::getWidth() const {
    return width;
}

int Power4Board::getHeight() const {
    return height;
}

void Power4Board::checkBounds(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("x or y out of range");
    }
}

short Power4Board::get(int x, int y) const {
    checkBounds(x, y);
    return board[x][y];
}

bool Power4Board::addInColumn(int column, short value) {
    if (column < 0 || column >= height) {
        throw std::out_of_range("column out of range");
    }
    for (int y = height - 1; y >= 0; y--) {
        if (board[y][column] == 0) {
            board[y][column] = value;
            return true;
        }
    }
    return false;
}

bool Power4Board::isFull() const {
    for (int x = 0; x < width; x++) {
        if (board[x][height - 1] == 0) {
            return false;
        }
    }
    return true;
}

short Power4Board::getWinner() const {
    // Horizontal
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width - 3; x++) {
            short value = board[y][x];
            if (value != 0 && value == board[y][x + 1] && value == board[y][x + 2] && value == board[y][x + 3]) {
                return value;
            }
        }
    }
    // Vertical
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height - 3; y++) {
            short value = board[y][x];
            if (value != 0 && value == board[y + 1][x] && value == board[y + 2][x] && value == board[y + 3][x]) {
                return value;
            }
        }
    }
    // Diagonal
    for (int x = 0; x < width - 3; x++) {
        for (int y = 0; y < height - 3; y++) {
            short value = board[y][x];
            if (value != 0 && value == board[y + 1][x + 1] && value == board[y + 2][x + 2] && value == board[y + 3][x + 3]) {
                return value;
            }
        }
    }
    for (int x = 0; x < width - 3; x++) {
        for (int y = 3; y < height; y++) {
            short value = board[y][x];
            if (value != 0 && value == board[y - 1][x + 1] && value == board[y - 2][x + 2] && value == board[y - 3][x + 3]) {
                return value;
            }
        }
    }
    return 0;
}

int Power4Board::count(bool (*predicate)(short)) const {
    int count = 0;
    for (const auto &column: board) {
        for (const auto &value: column) {
            if (predicate(value)) {
                count++;
            }
        }
    }
    return count;
}

int Power4Board::count(short value) const {
    for (auto &column: board) {
        for (auto &element: column) {
            if (element == value) {
                return true;
            }
        }
    }
    return false;
}

void Power4Board::print() const {
    for (char letter = 'A'; letter < 'A' + width; letter++) {
        std::cout << letter << " ";
    }
    std::cout << std::endl;
    for (const auto &column: board) {
        for (const auto &value: column) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}
