#ifndef TOUCHDOWN_GAME_STATE_H
#define TOUCHDOWN_GAME_STATE_H

#include "board.h"
#include "player.h"

#define NOT_YET_DECIDED 0
#define PLAYER_1 1
#define PLAYER_2 2
#define INVALID_MOVE_PERFORMED 3

namespace game {

    struct GameState {

        const Board m_board;
        const bool m_turn;   // true: Player 1, false: Player 2
        const unsigned char m_status; // 0: not yet decided, 1: Player 1 won, 2: Player 2 won, 3: Invalid move performed
        const game::Player* m_winner;
        const game::Player* m_player1;
        const game::Player* m_player2;

        bool isGameOver();

    };

}

#endif //TOUCHDOWN_GAME_STATE_H