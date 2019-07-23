#ifndef TOUCHDOWN_PLAYER_H
#define TOUCHDOWN_PLAYER_H

#include "board.h"
#include "move.h"

namespace game {

    struct Player {

        virtual const bool preferToStart(const Board& board) = 0;

        virtual const Move play(const Board& board) = 0;

    };

}

#endif //TOUCHDOWN_PLAYER_H