#ifndef TOUCHDOWN_PLAYER_STATISTICS_H
#define TOUCHDOWN_PLAYER_STATISTICS_H

#include <string>

namespace score {

    struct PlayerStatistics {

        std::string m_name;
        unsigned int m_wins;
        unsigned int m_losses;
        unsigned int m_ties;
        unsigned int m_total_moves;
        unsigned int m_total_time;

    };

}


#endif //TOUCHDOWN_PLAYER_STATISTICS_H
