#include "game_state.h"


namespace game {

    bool GameState::isGameOver() {
        return m_status != 0;
    }

}