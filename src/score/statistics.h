#ifndef TOUCHDOWN_STATISTICS_H
#define TOUCHDOWN_STATISTICS_H

#include "player_statistics.h"
#include "statistics_entry.h"
#include <vector>
#include <unordered_map>

namespace score {

    struct Statistics {

        std::unordered_map<std::string, PlayerStatistics> m_player_statistics;

        void clear();
        void print();
        void addEntry(const StatisticsEntry entry);

    private:

        std::vector<std::pair<std::string, PlayerStatistics>> getPlayerNames();
        unsigned int getNumberOfEntries();
        unsigned int getWins(std::string playerName);
        unsigned int getLosses(std::string playerName);
        unsigned int getTies(std::string playerName);

    };

}

#endif //TOUCHDOWN_STATISTICS_H
