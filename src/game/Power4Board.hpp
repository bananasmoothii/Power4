//
// Created by bananasmoothii on 21/07/2023.
//

#ifndef POWER4_POWER4BOARD_HPP
#define POWER4_POWER4BOARD_HPP


#include <vector>
#include <memory>
#include "Game.hpp"
#include "../util/Coord.hpp"
#include "color.hpp"

class Power4Board : public Game<unsigned short> {
private:
    int width;
    int height;
    std::unique_ptr<std::unique_ptr<unsigned short[]>[]> board;
    mutable std::vector<Coord> computedWinnerCoords;
    mutable bool isWinnerCoordsComputed = false;

    void checkBounds(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("x or y out of range");
        }
    }

    void setWinnerCoords(const std::vector<Coord> &coords) const {
        isWinnerCoordsComputed = true;
        computedWinnerCoords = coords;
    }

public:
    Power4Board(int width, int height) : width(width), height(height),
                                         board(std::make_unique<std::unique_ptr<unsigned short[]>[]>(height)) {
        if (width < 4 || height < 4) {
            throw std::invalid_argument("width or height too small");
        }
        for (int y = 0; y < height; y++) {
            board[y] = std::make_unique<unsigned short[]>(width);
            for (int x = 0; x < width; x++) {
                board[y][x] = 0;
            }
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
        checkBounds(x, y);
        return board[y][x];
    }

    /**
     * Adds a player to the column, returns true if successful, false if not
     */
    bool addInColumn(int column, unsigned short player) {
        if (player != 1 && player != 2) {
            throw std::invalid_argument("player must be 1 or 2");
        }
        if (column < 0 || column >= width) {
            throw std::out_of_range("column out of range");
        }
        for (int y = height - 1; y >= 0; y--) {
            if (board[y][column] == 0) {
                board[y][column] = player;
                isWinnerCoordsComputed = false;
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] double getScore(const unsigned short &player) const override {
        return 0; // TODO
    }

    [[nodiscard]] std::vector<unsigned short> getPlayers() const override {
        return {1, 2};
    }

    /**
     * Count elements with a given predicate, returning true or false for each element
     */
    int count(bool (*predicate)(unsigned short)) const {
        int count = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; ++y) {
                if (predicate(board[x][y])) {
                    count++;
                }
            }
        }
        return count;
    }

    [[nodiscard]] int count(unsigned short value) const {
        int count = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; ++y) {
                if (board[x][y] == value) {
                    count++;
                }
            }
        }
        return count;
    }

    [[nodiscard]] bool isDraw() const override {
        for (int x = 0; x < width; x++) {
            if (board[0][x] == 0) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] std::unique_ptr<unsigned short> getWinner() const override {
        std::vector<Coord> coords = getWinnerCoords();
        if (coords.empty()) return {nullptr};
        Coord coord0 = coords[0];
        return std::make_unique<unsigned short>(board[coord0.getY()][coord0.getX()]);
    }

    /**
     * This uses a cache.
     * @return a vector of all coords that are part of the winning line, sorted by ascending y then ascending x.
     * Empty if no winner.
     */
    [[nodiscard]] std::vector<Coord> getWinnerCoords() const {
        if (isWinnerCoordsComputed) {
            return computedWinnerCoords;
        }
        std::vector<Coord> coords;
        // Horizontal
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width - 3; x++) {
                unsigned short value = board[y][x];
                if (value != 0 && value == board[y][x + 1] && value == board[y][x + 2] && value == board[y][x + 3]) {
                    coords.emplace_back(x, y);
                    coords.emplace_back(x + 1, y);
                    coords.emplace_back(x + 2, y);
                    coords.emplace_back(x + 3, y);
                    setWinnerCoords(coords);
                    return coords;
                }
            }
        }
        // Vertical
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height - 3; y++) {
                unsigned short value = board[y][x];
                if (value != 0 && value == board[y + 1][x] && value == board[y + 2][x] && value == board[y + 3][x]) {
                    coords.emplace_back(x, y);
                    coords.emplace_back(x, y + 1);
                    coords.emplace_back(x, y + 2);
                    coords.emplace_back(x, y + 3);
                    setWinnerCoords(coords);
                    return coords;
                }
            }
        }
        // Diagonal
        for (int x = 0; x < width - 3; x++) {
            for (int y = 0; y < height - 3; y++) {
                unsigned short value = board[y][x];
                if (value != 0 && value == board[y + 1][x + 1] && value == board[y + 2][x + 2] &&
                    value == board[y + 3][x + 3]) {
                    coords.emplace_back(x, y);
                    coords.emplace_back(x + 1, y + 1);
                    coords.emplace_back(x + 2, y + 2);
                    coords.emplace_back(x + 3, y + 3);
                    setWinnerCoords(coords);
                    return coords;
                }
            }
        }
        for (int x = 0; x < width - 3; x++) {
            for (int y = 3; y < height; y++) {
                unsigned short value = board[y][x];
                if (value != 0 && value == board[y - 1][x + 1] && value == board[y - 2][x + 2] &&
                    value == board[y - 3][x + 3]) {
                    // we need to reverse the order of the coords so that they are sorted by ascending y then ascending x
                    coords.emplace_back(x + 3, y - 3);
                    coords.emplace_back(x + 2, y - 2);
                    coords.emplace_back(x + 1, y - 1);
                    coords.emplace_back(x, y);
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
        for (char letter = 'A'; letter < 'A' + width; letter++) {
            std::cout << letter << " ";
        }
        std::cout << std::endl;
        std::vector<Coord> winnerCoords = getWinnerCoords();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (!winnerCoords.empty() && winnerCoords[0] == Coord(x, y)) {
                    std::cout << dye::yellow(board[y][x]);
                    winnerCoords.erase(winnerCoords.begin());
                } else if (board[y][x] == 0) {
                    std::cout << dye::grey(board[y][x]);
                } else {
                    std::cout << board[y][x];
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
};


#endif //POWER4_POWER4BOARD_HPP
