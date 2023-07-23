//
// Created by bananasmoothii on 23/07/2023.
//

#ifndef POWER4_COORD_HPP
#define POWER4_COORD_HPP


class Coord {
private:
    int x;
    int y;
public:
    Coord(int x, int y) : x(x), y(y) {}

    [[nodiscard]] int getX() const {
        return x;
    }

    [[nodiscard]] int getY() const {
        return y;
    }

    bool operator==(const Coord &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    bool operator!=(const Coord &rhs) const {
        return !(*this == rhs);
    }

};


#endif //POWER4_COORD_HPP
