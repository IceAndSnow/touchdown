//
// Created by ask on 7/23/19.
//

#ifndef TOUCHDOWN_MOVE_H
#define TOUCHDOWN_MOVE_H

#include "board.h"

namespace game {

    class Move {

        unsigned char m_fromX, m_fromY;
        unsigned char m_toX, m_toY;

    public:

        Move(unsigned char fromX, unsigned char fromY, unsigned char toX, unsigned char toY);

        bool isValid(const Board& board, bool turn) const;

        void performMove(Board& board) const;

    };

}

#endif //TOUCHDOWN_MOVE_H
