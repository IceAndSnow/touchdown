#ifndef TOUCHDOWN_ICE_BOT_H
#define TOUCHDOWN_ICE_BOT_H

#include "../game/player.h"

namespace players {

    class IceBot : public game::Player {

        std::string name();

        bool preferToStart(const game::Board& board);

        game::Move play(const game::Board& board);

    };

}

#endif //TOUCHDOWN_ICE_BOT_H
