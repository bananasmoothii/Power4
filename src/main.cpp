#include <iostream>
#include "game/Power4Board.hpp"

int main() {
    Power4Board board;
    std::unique_ptr<unsigned short> winner = nullptr;
    unsigned short players = 2;
    unsigned short currentPlayer = 1;

    board.print();

    do {
        char columnLetter;
        std::cout << std::endl << "Player " << currentPlayer << ", enter a columnLetter: ";
        std::cin >> columnLetter;
        int column = columnLetter - 'A';
        if (column < 0 || column >= board.getWidth()) {
            std::cout << "Invalid columnLetter" << std::endl;
            continue;
        }
        if (!board.addInColumn(column, currentPlayer)) {
            std::cout << "Column full" << std::endl;
            continue;
        }
        winner = board.getWinner();
        currentPlayer = (currentPlayer % players) + 1;
        board.print();
    } while (!board.isDraw() && winner == nullptr);

    std::cout << std::endl;
    if (winner == nullptr) {
        std::cout << "Draw" << std::endl;
    } else {
        std::cout << "Winner: player " << *winner << std::endl;
    }

    return 0;
}
