#ifndef TOUCHDOWN_BOARD_H
#define TOUCHDOWN_BOARD_H

namespace game {

    class Board {

        char m_tiles[8][8];

    public:

        Board();

        const char* operator[](const unsigned int index) const;
        char* operator[](const unsigned int index);

        void initStandard();

        void print() const;

    };

}

#endif //TOUCHDOWN_BOARD_H