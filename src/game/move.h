#ifndef TOUCHDOWN_MOVE_H
#define TOUCHDOWN_MOVE_H

#include "board.h"

namespace game {

    struct Move {

        unsigned char m_fromX, m_fromY;
        unsigned char m_toX, m_toY;

        Move(unsigned char fromX, unsigned char fromY, unsigned char toX, unsigned char toY);

        bool isValid(const Board& board, bool turn) const;

        void performMove(Board& board) const;

    };

}

#endif //TOUCHDOWN_MOVE_H
