#ifndef TOUCHDOWN_STATISTICS_ENTRY_H
#define TOUCHDOWN_STATISTICS_ENTRY_H

#include "../game/board.h"
#include <string>

namespace score {

    struct StatisticsEntry {
        game::Board m_start_board;
        game::Board m_end_board;
        unsigned char m_end_status;
        std::string m_player1_name;
        std::string m_player2_name;
        unsigned int m_num_of_moves;
    };

}

#endif //TOUCHDOWN_STATISTICS_ENTRY_H
