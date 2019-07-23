#ifndef TOUCHDOWN_PLAYER_H
#define TOUCHDOWN_PLAYER_H

#include <string>
#include "board.h"
#include "move.h"

namespace game {

    struct Player {

        virtual std::string name() = 0;

        virtual bool preferToStart(const Board& board) = 0;

        virtual Move play(const Board& board) = 0;

    };

}

#endif //TOUCHDOWN_PLAYER_H