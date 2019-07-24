#include "game_state.h"


namespace game {

    GameState::GameState() { }

    bool GameState::isGameOver() {
        return m_status != 0;
    }

}