//
// Created by bananasmoothii on 21/07/2023.
//

#ifndef POWER4_POWER4BOARD_HPP
#define POWER4_POWER4BOARD_HPP


#include <vector>
#include <memory>
#include <queue>
#include <stack>
#include <array>
#include "Game.hpp"
#include "../util/Coord.hpp"
#include "color.hpp"


typedef unsigned char Power4Player;

class Power4Board : public Game<Power4Player> {
private:
    int width, height;
    std::vector<Power4Player> board;
    mutable std::stack<int> computedWinnerCoords;
    mutable bool isWinnerCoordsComputed = false;

    int getIndex(int x, int y) const {
        if (x < 0 || x >= width)
            throw std::out_of_range("x out of range, should be between 0 and " + std::to_string(width));
        if (y < 0 || y >= height)
            throw std::out_of_range("y out of range, should be between 0 and " + std::to_string(height));
        return y * width + x;
    }

    void setWinnerCoords(const std::stack<int> &coords) const {
        isWinnerCoordsComputed = true;
        computedWinnerCoords = coords;
    }

public:

    Power4Board(int width, int height) : width(width), height(height), board(width * height, '0') {
        if (width < 4 || height < 4) {
            throw std::invalid_argument("width or height too small");
        }
    }

    Power4Board() : Power4Board(7, 6) {}

    [[nodiscard]] int getWidth() const {
        return width;
    }

    [[nodiscard]] int getHeight() const {
        return height;
    }

    [[nodiscard]] unsigned short get(int x, int y) const {
        return board[getIndex(x, y)];
    }

    /**
     * Adds a player to the column, returns true if successful, false if not
     */
    bool addInColumn(int column, Power4Player player) {
        if (player != '1' && player != '2') {
            throw std::invalid_argument("player must be 1 or 2");
        }
        if (column < 0 || column >= width) {
            throw std::out_of_range("column out of range");
        }
        for (int y = height - 1; y >= 0; y--) {
            int index = getIndex(column, y);
            if (board[index] == '0') {
                board[index] = player;
                isWinnerCoordsComputed = false;
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] double getScore(const Power4Player &player) const override {
        return 0; // TODO
    }

    [[nodiscard]] std::vector<Power4Player> getPlayers() const override {
        return {'1', '2'};
    }

    /**
     * Count elements with a given predicate, returning true or false for each element
     */
    template<typename P>
    int count(P &&predicate) const {
        int count = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; ++y) {
                if (predicate(board[getIndex(x, y)])) {
                    count++;
                }
            }
        }
        return count;
    }

    [[nodiscard]] int count(Power4Player value) const {
        int count = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; ++y) {
                if (board[getIndex(x, y)] == value) {
                    count++;
                }
            }
        }
        return count;
    }

    [[nodiscard]] bool isDraw() const override {
        for (int x = 0; x < width; x++) {
            if (board[getIndex(x, 0)] == '0') {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] std::unique_ptr<Power4Player> getWinner() const override {
        std::stack<int> coords = getWinnerCoords();
        if (coords.empty()) return {nullptr};
        int coord0 = coords.top();
        return std::make_unique<Power4Player>(board[coord0]);
    }

    /**
     * This uses a cache.
     * @return a vector of all coords that are part of the winning line, sorted by descending x then descending y,
     * meaning that std::queue::top() will return the coords with the lowest x then lowest y.
     * Empty if no winner.
     */
    [[nodiscard]] std::stack<int> getWinnerCoords() const {
        if (isWinnerCoordsComputed) {
            return computedWinnerCoords;
        }
        std::stack<int> coords;
        // Horizontal
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width - 3; x++) {
                Power4Player value = board[getIndex(x, y)];
                if (value != '0' && value == board[getIndex(x + 1, y)] && value == board[getIndex(x + 2, y)] &&
                    value == board[getIndex(x + 3, y)]) {
                    coords.emplace(getIndex(x + 3, y));
                    coords.emplace(getIndex(x + 2, y));
                    coords.emplace(getIndex(x + 1, y));
                    coords.emplace(getIndex(x, y));
                    setWinnerCoords(coords);
                    return coords;
                }
            }
        }
        // Vertical
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height - 3; y++) {
                Power4Player value = board[getIndex(x, y)];
                if (value != '0' && value == board[getIndex(x, y + 1)] && value == board[getIndex(x, y + 2)] &&
                    value == board[getIndex(x, y + 3)]) {
                    coords.emplace(getIndex(x, y + 3));
                    coords.emplace(getIndex(x, y + 2));
                    coords.emplace(getIndex(x, y + 1));
                    coords.emplace(getIndex(x, y));
                    setWinnerCoords(coords);
                    return coords;
                }
            }
        }
        // Diagonal
        for (int x = 0; x < width - 3; x++) {
            for (int y = 0; y < height - 3; y++) {
                Power4Player value = board[getIndex(x, y)];
                if (value != '0' && value == board[getIndex(x + 1, y + 1)] && value == board[getIndex(x + 2, y + 2)] &&
                    value == board[getIndex(x + 3, y + 3)]) {
                    coords.emplace(getIndex(x + 3, y + 3));
                    coords.emplace(getIndex(x + 2, y + 2));
                    coords.emplace(getIndex(x + 1, y + 1));
                    coords.emplace(getIndex(x, y));
                    setWinnerCoords(coords);
                    return coords;
                }
            }
        }
        for (int x = 0; x < width - 3; x++) {
            for (int y = 3; y < height; y++) {
                Power4Player value = board[getIndex(x, y)];
                if (value != '0' && value == board[getIndex(x + 1, y - 1)] && value == board[getIndex(x + 2, y - 2)] &&
                    value == board[getIndex(x + 3, y - 3)]) {
                    coords.emplace(getIndex(x, y));
                    coords.emplace(getIndex(x + 1, y - 1));
                    coords.emplace(getIndex(x + 2, y - 2));
                    coords.emplace(getIndex(x + 3, y - 3));
                    setWinnerCoords(coords);
                    return coords;
                }
            }
        }
        setWinnerCoords(coords);
        return coords;
    }

    /**
     * Prints the board to stdout
     */
    void print() const {
        std::cout << std::endl;
        for (Power4Player letter = 'A'; letter < 'A' + width; letter++) {
            std::cout << letter << " ";
        }
        std::cout << std::endl;
        std::stack<int> winnerCoords = getWinnerCoords();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Power4Player value = board[getIndex(x, y)];
                if (!winnerCoords.empty() && winnerCoords.top() == getIndex(x, y)) {
                    std::cout << dye::yellow(value);
                    winnerCoords.pop();
                } else {
                    switch (value) {
                        case '0':
                            std::cout << dye::grey(value);
                            break;
                        case '1':
                            std::cout << dye::green(value);
                            break;
                        case '2':
                            std::cout << dye::blue(value);
                            break;
                        default:
                            std::cout << value;
                    }
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
};


#endif //POWER4_POWER4BOARD_HPP
