//
// Created by bananasmoothii on 21/07/2023.
//

#ifndef POWER4_POWER4BOARD_HPP
#define POWER4_POWER4BOARD_HPP


#include <vector>

class Power4Board {
private:
    int width;
    int height;
    std::vector<std::vector<short>> board;

    void checkBounds(int x, int y) const;

public:
    Power4Board(int width, int height);

    Power4Board() : Power4Board(7, 6) {}

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] short get(int x, int y) const;

    [[nodiscard]] bool isFull() const;

    /**
     * Returns the winner, 0 if none
     */
    [[nodiscard]] short getWinner() const;

    /**
     * Adds a value to the column, returns true if successful, false if not
     */
    bool addInColumn(int column, short value);
    
    /**
     * Count elements with a given predicate, returning true or false for each element
     */
    int count(bool (*predicate)(short)) const;

    [[nodiscard]] int count(short value) const;

    /**
     * Prints the board to stdout
     */
    void print() const;
};


#endif //POWER4_POWER4BOARD_HPP
