#include "random_bot.h"
#include <cstdlib>
#include <ctime>

namespace players {

    RandomBot::RandomBot() {
        srand(time(0));
    }

    RandomBot::RandomBot(unsigned int seed) {
        srand(seed);
    }

    std::string RandomBot::name() const {
        return "Random Bot v1";
    }

    bool RandomBot::preferToStart(const game::Board &board) {
        return rand() % 2 == 0; // Randomly choose to start or not
    }

    game::Move RandomBot::play(const game::Board &board) {
        game::Move move(rand() % 8, rand() % 8, rand() % 8, rand() % 8);
        while(!move.isValid(board, true)) {
            move = game::Move(rand() % 8, rand() % 8, rand() % 8, rand() % 8);
        }
        return move;
    }

}