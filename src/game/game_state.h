#ifndef TOUCHDOWN_GAME_STATE_H
#define TOUCHDOWN_GAME_STATE_H

#include "board.h"

#define NOT_YET_DECIDED 0
#define PLAYER_1 1
#define PLAYER_2 2
#define INVALID_MOVE_PERFORMED 3

namespace game {

    struct GameState {

        const Board m_board;
        const bool m_turn;   // true: Player 1, false: Player 2
        const unsigned char m_winner; // 0: not yet decided, 1: Player 1, 2: Player 2, 3: Invalid move performed

    public:

        bool isGameOver();

    };

}

#endif //TOUCHDOWN_GAME_STATE_H