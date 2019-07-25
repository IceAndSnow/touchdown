#ifndef TOUCHDOWN_HIGH_SCORE_H
#define TOUCHDOWN_HIGH_SCORE_H

#include "../game/game.h"
#include "statistics.h"
#include <vector>

namespace score {

    struct HighScore {

        Statistics m_statistics;

        Statistics recordTournament(std::vector<game::Player*> players, unsigned int botVsBotTimes = 10);
        Statistics recordTournament(std::vector<game::Player*> players, game::Board board, unsigned int botVsBotTimes = 10);

        void resetStatistics();
        void printStatistics();

    };

}

#endif //TOUCHDOWN_HIGH_SCORE_H
