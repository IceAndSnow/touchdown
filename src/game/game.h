#ifndef TOUCHDOWN_GAME_H
#define TOUCHDOWN_GAME_H

#include "player.h"
#include "board.h"
#include "game_state.h"

namespace game {

    class Game {

        Board m_board;
        Player* m_players[2];
        bool m_turn;

    public:

        Game(Player* player1, Player* player2);

        void loadBoard(Board board);

        GameState performNextMove();

    };

}

#endif //TOUCHDOWN_GAME_H
