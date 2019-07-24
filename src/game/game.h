#ifndef TOUCHDOWN_GAME_H
#define TOUCHDOWN_GAME_H

#include "player.h"
#include "board.h"
#include "game_state.h"

namespace game {

    class Game {

        GameState m_state;

    public:

        Game(Player* player1, Player* player2);

        void loadBoard(Board board);

        GameState performNextMove();

        GameState getCurrentState();

    };

}

#endif //TOUCHDOWN_GAME_H
