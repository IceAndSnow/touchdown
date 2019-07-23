#include <string.h>
#include "board.h"


namespace game {

    Board::Board() {
        memset(m_tiles, 0, sizeof(m_tiles));
    }

    const char* Board::operator[](const unsigned int index) const {
        return m_tiles[index];
    }

    char* Board::operator[](const unsigned int index) {
        return m_tiles[index];
    }

}