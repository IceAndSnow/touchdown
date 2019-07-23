#include <cstdlib>
#include "move.h"

namespace game {

    Move::Move(unsigned char fromX, unsigned char fromY, unsigned char toX, unsigned char toY) {
        m_fromX = fromX;
        m_fromY = fromY;
        m_toX = toX;
        m_toY = toY;
    }

    inline static bool isOutOfBounds(unsigned char x, unsigned char y) {
        return x >= 8 || y >= 8;
    }

    bool Move::isValid(const game::Board &board, bool turn) const {
        if(isOutOfBounds(m_fromX, m_fromY)) {
            return false;
        }
        if(isOutOfBounds(m_toX, m_toY)) {
            return false;
        }

        if(turn && board[m_fromX][m_fromY] != 1) {
            return false;
        }
        if(!turn && board[m_fromX][m_fromY] != 2) {
            return false;
        }

        if(turn && m_toY != m_fromY - 1) {
            return false;
        }
        if(!turn && m_toY != m_fromY + 1) {
            return false;
        }

        char dx = (char)m_toX - (char)m_fromX;

        if(abs(dx) > 1) {
            return false;
        }
        if(abs(dx) == 1) {
            if(turn && board[m_toX][m_toY] != 2) {
                return false;
            }
            if(!turn && board[m_toX][m_toY] != 1) {
                return false;
            }
        }
        if(board[m_toX][m_toY] != 0) {
            return false;
        }

        return true;
    }

    void Move::performMove(game::Board &board) const {
        board[m_toX][m_toY] = board[m_fromX][m_fromY];
        board[m_fromX][m_fromY] = 0;
    }

}