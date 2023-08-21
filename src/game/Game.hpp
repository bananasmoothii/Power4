//
// Created by bananasmoothii on 22/07/2023.
//

#ifndef POWER4_GAME_HPP
#define POWER4_GAME_HPP


#include <vector>
#include <memory>

template<typename Player>
class Game {
    /**
     * Returns the score of the player, higher is better.
     *
     * If there are only two players, the score of the first player is the opposite of the score of the second player.
     */
    virtual double getScore(const Player &player) const = 0;

    /**
     * @return a vector of all players
     */
    virtual std::vector<Player> getPlayers() const = 0;

    /**
     * @return true if the game is a draw (and over), false if not
     */
    [[nodiscard]] virtual bool isDraw() const = 0;

    /**
     * Returns the winner, nullptr if none.
     * If there is a winner, the game is over.
     */
    virtual std::unique_ptr<Player> getWinner() const = 0;
};


#endif //POWER4_GAME_HPP
