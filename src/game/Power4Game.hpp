//
// Created by bananasmoothii on 21/07/2023.
//

#ifndef POWER4_POWER4GAME_HPP
#define POWER4_POWER4GAME_HPP


#include <vector>
#include <memory>
#include <queue>
#include <stack>
#include <array>
#include <limits>
#include <format>
#include "Game.hpp"
#include "../util/Coord.hpp"
#include "../util/MathUtils.hpp"
#include "color.hpp"
#include "../util/OutOfRangeException.hpp"


typedef unsigned char Power4Player;

class Power4Game : public Game<Power4Player> {
private:
    unsigned int width, height;
    std::vector<Power4Player> board;
    mutable std::stack<unsigned int> computedWinnerCoords;
    mutable bool isWinnerCoordsComputed = false;

    unsigned int getIndex(unsigned int x, unsigned int y) const {
        if (x >= width)
            throw OutOfRangeException(
                    "x out of range, should be between 0 and " + std::to_string(width) + ", was " + std::to_string(x));
        if (y >= height)
            throw OutOfRangeException(
                    "y out of range, should be between 0 and " + std::to_string(height) + ", was " + std::to_string(y));
        return y * width + x;
    }

    void setWinnerCoords(const std::stack<unsigned int> &coords) const {
        isWinnerCoordsComputed = true;
        computedWinnerCoords = coords;
    }

public:

    Power4Game(int width, int height) : width(width), height(height), board(width * height, '0') {
        if (width < 4 || height < 4) {
            throw std::invalid_argument("width or height too small");
        }
    }

    Power4Game() : Power4Game(7, 6) {}

    [[nodiscard]] unsigned int getWidth() const {
        return width;
    }

    [[nodiscard]] unsigned int getHeight() const {
        return height;
    }

    [[nodiscard]] Power4Player get(unsigned int x, unsigned int y) const {
        return board[getIndex(x, y)];
    }

    [[nodiscard]] Power4Player get(int x, int y) const {
        if (x < 0) throw OutOfRangeException("x out of range, should be positive, was " + std::to_string(x));
        if (y < 0) throw OutOfRangeException("y out of range, should be positive, was " + std::to_string(y));
        return board[getIndex(static_cast<unsigned int>(x), static_cast<unsigned int>(y))];
    }

    /**
     * Adds a player to the column, returns true if successful, false if not
     */
    bool addInColumn(unsigned int column, Power4Player player) {
        if (player != '1' && player != '2') {
            throw std::invalid_argument("player must be 1 or 2");
        }
        if (column >= width) {
            throw std::out_of_range("column out of range");
        }
        for (unsigned int y = height - 1;
             y <= height; y--) { // loop will end when y underflows to the max value of unsigned int
            unsigned int index = getIndex(column, y);
            if (board[index] == '0') {
                board[index] = player;
                isWinnerCoordsComputed = false;
                return true;
            }
        }
        return false;
    }

    enum IteratorType {
        HORIZONTAL,
        VERTICAL,
        DIAGONAL_DOWN,
        DIAGONAL_UP
    };

    static constexpr std::array<IteratorType, 4> iteratorTypes = {HORIZONTAL, VERTICAL, DIAGONAL_DOWN, DIAGONAL_UP};

    class BoardIterator {
    private:
        const Power4Game *game;
        const IteratorType iteratorType;
        int x, y; // current coordinates

    public:
        BoardIterator(const Power4Game *game, const IteratorType iteratorType, const int startX, const int startY)
                : game(game), iteratorType(iteratorType), x(startX), y(startY) {}

        BoardIterator(const Power4Game *game, const IteratorType iteratorType)
                : game(game), iteratorType(iteratorType), x(0), y(0) {}

        [[nodiscard]] IteratorType getIteratorType() const {
            return iteratorType;
        }

        [[nodiscard]] bool isInBoard() const {
            return x >= 0 && x < static_cast<int>(game->getWidth()) && y >= 0 &&
                   y < static_cast<int>(game->getHeight());
        }

        Power4Player operator*() const {
            // Deference operator
            return game->get(x, y);
        }

        [[nodiscard]] Power4Player getOrEmpty() const {
            if (isInBoard()) {
                return game->get(x, y);
            }
            return 0;
        }

        BoardIterator &operator++() {
            // Prefix increment operator
            switch (iteratorType) {
                case HORIZONTAL:
                    x++;
                    break;
                case VERTICAL:
                    y++;
                    break;
                case DIAGONAL_DOWN:
                    x++;
                    y++;
                    break;
                case DIAGONAL_UP:
                    x++;
                    y--;
                    break;
            }
            return *this;
        }

        BoardIterator &operator--() {
            // Prefix decrement operator
            switch (iteratorType) {
                case HORIZONTAL:
                    x--;
                    break;
                case VERTICAL:
                    y--;
                    break;
                case DIAGONAL_DOWN:
                    x--;
                    y--;
                    break;
                case DIAGONAL_UP:
                    x--;
                    y++;
                    break;
            }
            return *this;
        }

        bool operator!=(const BoardIterator &other) const {
            return x != other.x || y != other.y || iteratorType != other.iteratorType;
        }

        bool operator==(const BoardIterator &other) const {
            return !(*this != other);
        }
    };

private:
    static constexpr double WIN_SCORE = std::numeric_limits<double>::infinity();
    static constexpr double SCORE_3_ALIGNED = 10;
    static constexpr double SCORE_2_ALIGNED = 5;

    [[nodiscard]] static double calculateScore(unsigned int nb2Aligned, unsigned int nb3Aligned) {
        return SCORE_2_ALIGNED * nb2Aligned + (nb3Aligned == 0 ? 0 : intPow(SCORE_3_ALIGNED, nb3Aligned));
    }

public:
    /**
     * Returns the score of the player, higher is better
     *
     * Scores:
     * - 2 aligned: 5n (n = number of 2 aligned)
     * - 3 aligned: 10^n (n = number of 3 aligned)
     * - 4 aligned: infinite
     * Subtract the same score for the opponent
     */
    [[nodiscard]] double getScore(const Power4Player &player) const override {
        unsigned int p1Aligns2 = 0;
        unsigned int p1Aligns3 = 0;
        unsigned int p2Aligns2 = 0;
        unsigned int p2Aligns3 = 0;
        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {
                for (const IteratorType &iteratorType: iteratorTypes) {
                    int startX = static_cast<int>(x), startY = static_cast<int>(y);
                    // we offset the iterators by -2 to be able to retrieve elements before the actual point
                    switch (iteratorType) {
                        case HORIZONTAL:
                            startX -= 2;
                            break;
                        case VERTICAL:
                            startY -= 2;
                            break;
                        case DIAGONAL_DOWN:
                            startX -= 2;
                            startY -= 2;
                            break;
                        case DIAGONAL_UP:
                            startX -= 2;
                            startY += 2;
                            break;
                    }
                    BoardIterator boardIterator{this, iteratorType, startX, startY};
                    const bool hasSpace2Before = boardIterator.getOrEmpty() == '0';
                    const bool hasSpaceBefore = (++boardIterator).getOrEmpty() == '0';

                    const Power4Player current = (++boardIterator).getOrEmpty();
                    if (current != '1' && current != '2') continue;

                    unsigned int *currentAligns2Ptr;
                    unsigned int *currentAligns3Ptr;
                    if (current == '1') {
                        currentAligns2Ptr = &p1Aligns2;
                        currentAligns3Ptr = &p1Aligns3;
                    } else {
                        currentAligns2Ptr = &p2Aligns2;
                        currentAligns3Ptr = &p2Aligns3;
                    }

                    const Power4Player second = (++boardIterator).getOrEmpty();
                    if (second != current) continue;
                    const Power4Player third = (++boardIterator).getOrEmpty();
                    const Power4Player fourth = (++boardIterator).getOrEmpty();

                    // we know that second == current
                    if (third == current) { // we have 3 aligned
                        if (fourth == current) { // we have 4 aligned
                            return current == '1' ? WIN_SCORE : -WIN_SCORE;
                        }
                        if (fourth == '0') { // we have space ahead
                            ++(*currentAligns3Ptr);
                        }
                        if (hasSpaceBefore) {
                            ++(*currentAligns3Ptr);
                        }
                    } else { // we have 2 aligned
                        if (third == '0' && fourth == '0') { // we have space ahead to add 2
                            ++(*currentAligns2Ptr);
                        }
                        if (hasSpaceBefore && hasSpace2Before) {
                            ++(*currentAligns2Ptr);
                        }
                    }

/*
                    while (current == playerAtIteratorStart) {
                        switch (i) {
                            case 2:
                                if (hasSpaceBefore) { // invalid if the player cannot continue
                                    ++(*currentAligns2Ptr);
                                }
                                if (next == '0') { // if two sides are free, there are two possibilities to continue
                                    ++(*currentAligns2Ptr);
                                }
                                break;
                            case 3:
                                if (hasSpaceBefore) {
                                    ++(*currentAligns3Ptr);
                                    --(*currentAligns2Ptr); // 3 aligned is also 2 aligned
                                }
                                if (next == '0') {
                                    ++(*currentAligns3Ptr);
                                    --(*currentAligns2Ptr); // we are removing 2 2aligned because 2 were added in the previous iteration
                                }
                                break;
                            case 4:
                                return player == playerAtIteratorStart ? WIN_SCORE : -WIN_SCORE;
                            default:
                                break;
                        }

                        if (!boardIterator.isInBoard()) break; // if future current is not in board
                        ++i;
                        ++boardIterator;
                        current = next;
                        next = boardIterator.getOrEmpty();
                    }
*/
                }
            }
        }
        double p1Score = calculateScore(p1Aligns2, p1Aligns3) - calculateScore(p2Aligns2, p2Aligns3);
        return player == '1' ? p1Score : -p1Score;
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
        for (const auto &item: board) {
            if (predicate(item)) {
                count++;
            }
        }
        return count;
    }

    [[nodiscard]] int count(Power4Player value) const {
        int count = 0;
        for (const auto &item: board) {
            if (item == value) {
                count++;
            }
        }
        return count;
    }

    [[nodiscard]] bool isDraw() const override {
        for (unsigned int x = 0; x < width; x++) {
            if (board[getIndex(x, 0)] == '0') {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] std::unique_ptr<Power4Player> getWinner() const override {
        std::stack<unsigned int> coords = getWinnerCoords();
        if (coords.empty()) return {nullptr};
        unsigned int coord0 = coords.top();
        return std::make_unique<Power4Player>(board[coord0]);
    }

    /**
     * This uses a cache.
     * @return a vector of all coords that are part of the winning line, sorted by descending x then descending y,
     * meaning that std::queue::top() will return the coords with the lowest x then lowest y.
     * Empty if no winner.
     */
    [[nodiscard]] std::stack<unsigned int> getWinnerCoords() const {
        if (isWinnerCoordsComputed) {
            return computedWinnerCoords;
        }
        std::stack<unsigned int> coords;
        // Horizontal
        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width - 3; x++) {
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
        for (unsigned int x = 0; x < width; x++) {
            for (unsigned int y = 0; y < height - 3; y++) {
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
        for (unsigned int x = 0; x < width - 3; x++) {
            for (unsigned int y = 0; y < height - 3; y++) {
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
        for (unsigned int x = 0; x < width - 3; x++) {
            for (unsigned int y = 3; y < height; y++) {
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
        std::stack<unsigned int> winnerCoords = getWinnerCoords();
        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {
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
        printScores();
    }

    void printScores() const {
        double p1Score = getScore('1');
        std::cout << "Scores: " << dye::green(std::format("1: {:.1f}", p1Score)) << " "
                  << dye::blue(std::format("2: {:.1f}", -p1Score)) << std::endl;
    }
};


#endif //POWER4_POWER4GAME_HPP
