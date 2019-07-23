#include <iostream>
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

    void Board::initStandard() {
        memset(m_tiles, 0, sizeof(m_tiles));
        for(unsigned char x = 0; x < 8; ++x) {
            m_tiles[x][1] = 2;
            m_tiles[x][6] = 1;
        }
    }

    void Board::print() const {
        std::cout << "------------------------------" << std::endl;
        for(unsigned char x = 0; x < 8; ++x) {
            std::cout << "|";
            for(unsigned char y = 0; y < 8; ++y) {
                std::cout << " " << (int)m_tiles[x][y] << " ";
            }
            std::cout << "|" << std::endl;
        }
        std::cout << "------------------------------" << std::endl;
    }

}