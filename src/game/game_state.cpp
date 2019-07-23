#include "game_state.h"


namespace game {

    bool GameState::isGameOver() {
        return m_winner != 0;
    }

}