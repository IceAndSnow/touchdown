#ifndef TOUCHDOWN_RANDOM_BOT_H
#define TOUCHDOWN_RANDOM_BOT_H

#include "../game/player.h"

namespace players {

    class RandomBot : public game::Player {

    public:

        RandomBot();
        RandomBot(unsigned int seed);

        std::string name();

        bool preferToStart(const game::Board& board);

        game::Move play(const game::Board& board);

    };

}

#endif //TOUCHDOWN_RANDOM_BOT_H
