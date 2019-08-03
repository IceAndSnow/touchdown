#include "statistics.h"
#include "../options.h"
#include "../game/game.h"
#include <iostream>
#include <algorithm>

namespace score {

    void Statistics::clear() {
        m_player_statistics.clear();
    }

    void Statistics::addEntry(const score::StatisticsEntry entry) {
        if(m_player_statistics.find(entry.m_player1_name) == m_player_statistics.end()) {
            m_player_statistics[entry.m_player1_name] = {
                    entry.m_player1_name,
                    0,
                    0,
                    0,
                    0
            };
        }
        if(m_player_statistics.find(entry.m_player2_name) == m_player_statistics.end()) {
            m_player_statistics[entry.m_player2_name] = {
                    entry.m_player2_name,
                    0,
                    0,
                    0,
                    0
            };
        }

        m_player_statistics[entry.m_player1_name].m_total_time += entry.m_total_millis_player1;
        m_player_statistics[entry.m_player2_name].m_total_time += entry.m_total_millis_player2;

        m_player_statistics[entry.m_player2_name].m_total_moves += entry.m_num_of_moves / 2;

        if(entry.m_num_of_moves % 2 == 0) {
            m_player_statistics[entry.m_player1_name].m_total_moves += entry.m_num_of_moves / 2;
        } else {
            m_player_statistics[entry.m_player1_name].m_total_moves += entry.m_num_of_moves / 2 + 1;
        }

        if(entry.m_end_status == PLAYER_1) {
            ++m_player_statistics[entry.m_player1_name].m_wins;
            ++m_player_statistics[entry.m_player2_name].m_losses;
        } else if(entry.m_end_status == PLAYER_2) {
            ++m_player_statistics[entry.m_player1_name].m_losses;
            ++m_player_statistics[entry.m_player2_name].m_wins;
        } else if(entry.m_end_status == TIE) {
            ++m_player_statistics[entry.m_player1_name].m_ties;
            ++m_player_statistics[entry.m_player2_name].m_ties;
        }
    }

    unsigned int Statistics::getWins(std::string playerName) {
        return m_player_statistics[playerName].m_wins;
    }

    unsigned int Statistics::getLosses(std::string playerName) {
        return m_player_statistics[playerName].m_losses;
    }

    unsigned int Statistics::getTies(std::string playerName) {
        return m_player_statistics[playerName].m_ties;
    }

    static bool mapEntryComparator(
            const std::pair<std::string, PlayerStatistics>& a,
            const std::pair<std::string, PlayerStatistics>& b) {
        return a.second.m_wins>b.second.m_wins;
    }

    std::vector<std::pair<std::string, PlayerStatistics>> Statistics::getPlayerNames() {
        std::vector<std::pair<std::string, PlayerStatistics>> result(m_player_statistics.begin(), m_player_statistics.end());
        std::sort(result.begin(), result.end(), mapEntryComparator);
        return result;
    }

    unsigned int Statistics::getNumberOfEntries() {
        unsigned int result = 0;
        for(auto& pair : m_player_statistics) {
            result += pair.second.m_wins + pair.second.m_losses + pair.second.m_ties;
        }
        return result / 2;
    }

    void Statistics::print() {
        std::vector<std::pair<std::string, PlayerStatistics>> playerStatistics = getPlayerNames();

        std::string nameHeader = "Player name";
        unsigned int nameWidth = 30;

        std::string winsHeader = "Wins";
        unsigned int winsWidth = 10;

        std::string lossesHeader = "Losses";
        unsigned int lossesWidth = 10;

        std::string tiesHeader = "Ties";
        unsigned int tiesWidth = 10;

        std::string avgTimeHeader = "Avg. Time Spent (ms)";
        unsigned int avgTimeWidth = 20;


        std::cout << "Statistics (" << getNumberOfEntries() << " entries):" << std::endl;
        for(unsigned int i = 0; i < nameWidth + winsWidth + lossesWidth + tiesWidth + avgTimeWidth + 16; ++i) {
            std::cout << "=";
        }
        std::cout << std::endl;
        printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n", nameWidth, nameHeader.c_str(), winsWidth, winsHeader.c_str(), lossesWidth, lossesHeader.c_str(), tiesWidth, tiesHeader.c_str(), avgTimeWidth, avgTimeHeader.c_str());
        for(unsigned int i = 0; i < nameWidth + winsWidth + lossesWidth + tiesWidth + avgTimeWidth + 16; ++i) {
            std::cout << "=";
        }
        std::cout << std::endl;
        for(unsigned int player_index = 0; player_index < playerStatistics.size(); ++player_index) {
            if(player_index != 0) {
                for(unsigned int i = 0; i < nameWidth + winsWidth + lossesWidth + tiesWidth + avgTimeWidth + 16; ++i) {
                    std::cout << "-";
                }
                std::cout << std::endl;
            }
            std::string playerName = playerStatistics[player_index].first;
            unsigned int wins = playerStatistics[player_index].second.m_wins;
            unsigned int losses = playerStatistics[player_index].second.m_losses;
            unsigned int ties = playerStatistics[player_index].second.m_ties;
            unsigned int avgTime = playerStatistics[player_index].second.m_total_time / playerStatistics[player_index].second.m_total_moves;
            printf("| %-*s | %-*d | %-*d | %-*d | %-*d |\n", nameWidth, playerName.c_str(), winsWidth, wins, lossesWidth, losses, tiesWidth, ties, avgTimeWidth, avgTime);
        }
        for(unsigned int i = 0; i < nameWidth + winsWidth + lossesWidth + tiesWidth + avgTimeWidth + 16; ++i) {
            std::cout << "=";
        }
        std::cout << std::endl;
    }


    void Statistics::eprint() {
        std::vector<std::pair<std::string, PlayerStatistics>> playerStatistics = getPlayerNames();

        EXP_PRINT(getNumberOfEntries());

        for(unsigned int player_index = 0; player_index < playerStatistics.size(); ++player_index) {
            std::string playerName = playerStatistics[player_index].first;
            unsigned int wins = playerStatistics[player_index].second.m_wins;
            unsigned int losses = playerStatistics[player_index].second.m_losses;
            unsigned int ties = playerStatistics[player_index].second.m_ties;
            unsigned int avgTime = playerStatistics[player_index].second.m_total_time / playerStatistics[player_index].second.m_total_moves;
            EXP_PRINT(";" << playerName << ";" << wins << ";" << losses << ";" << ties << ";" << avgTime);
        }
    }

}