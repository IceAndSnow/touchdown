#ifndef TOUCHDOWN_HIGH_SCORE_H
#define TOUCHDOWN_HIGH_SCORE_H

#include "../game/game.h"
#include "../players/player_instantiator.h"
#include "statistics.h"
#include <vector>

namespace score {

    struct HighScore {

        Statistics m_statistics;

        Statistics recordTournament(std::vector<players::PlayerInstantiator*> playerGens, unsigned int botVsBotTimes = 10);
        Statistics recordTournament(std::vector<players::PlayerInstantiator*> playerGens, game::Board board, unsigned int botVsBotTimes = 10);

        void resetStatistics();
        void printStatistics();

    };

}

#endif //TOUCHDOWN_HIGH_SCORE_H
