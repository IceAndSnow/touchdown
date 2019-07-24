#ifndef TOUCHDOWN_ICE_BOT_H
#define TOUCHDOWN_ICE_BOT_H

#include "../game/player.h"

namespace players {

    class IceBot : public game::Player {

    public:

        std::string name() const;

        bool preferToStart(const game::Board& board);

        game::Move play(const game::Board& board);

    };

}

#endif //TOUCHDOWN_ICE_BOT_H
