#ifndef TOUCHDOWN_ICE_SPEAR_BOT_H
#define TOUCHDOWN_ICE_SPEAR_BOT_H

#include "../game/player.h"
#include <vector>
#include <random>

namespace players {

    typedef std::pair<int, game::Move>  IceSmashScore;

    class IceSmashBot : public game::Player {

        std::default_random_engine m_randEngine;

        std::vector<game::Move> getAllValidMoves(const game::Board& board) const;
        std::vector<IceSmashScore> getAllScores(const game::Board& board, const std::vector<game::Move> moves) const;
        IceSmashScore getScoreIfMove(const game::Board& board, const game::Move move) const;
        int getScore(const game::Board& board) const;
        int findMoveAhead(const game::Board& board, unsigned int x, unsigned int y, int dir) const;

    public:

        IceSmashBot();
        IceSmashBot(unsigned int seed);

        std::string name() const;

        bool preferToStart(const game::Board& board);

        game::Move play(const game::Board& board);

    };

}

#endif //TOUCHDOWN_ICE_SPEAR_BOT_H