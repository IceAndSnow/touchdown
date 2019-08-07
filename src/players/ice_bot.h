#ifndef TOUCHDOWN_ICE_BOT_H
#define TOUCHDOWN_ICE_BOT_H

#include "../grundy/grundy.h"
#include "../game/player.h"

namespace players {

    typedef std::pair<unsigned char, unsigned char> pos;

    class IceBot : public game::Player {

        grundy::Grundy m_grundy;

        unsigned int partialGrundy(const game::Board& board, bool turn, pos p);

        game::Move getOptimalPartialGrundyMove(game::Board board, pos p);

        game::Move partialGrundyAlgorithm(const game::Board& board);

    public:

        std::string name() const;

        bool preferToStart(const game::Board& board);

        game::Move play(const game::Board& board);

    };

}

#endif //TOUCHDOWN_ICE_BOT_H
