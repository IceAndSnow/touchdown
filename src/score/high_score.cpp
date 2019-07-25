#include "high_score.h"

namespace score {

    void HighScore::printStatistics() {
        m_statistics.print();
    }

    void HighScore::resetStatistics() {
        m_statistics.clear();
    }

    Statistics HighScore::recordTournament(std::vector<game::Player *> players, unsigned int botVsBotTimes) {
        game::Board defaultBoard;
        defaultBoard.initStandard();
        return recordTournament(players, defaultBoard, botVsBotTimes);
    }

    Statistics HighScore::recordTournament(std::vector<game::Player *> players, game::Board board, unsigned int botVsBotTimes) {
        for(unsigned int b1 = 0; b1 < players.size(); ++b1) {
            if(players[b1] == nullptr) {
                continue;
            }
            for(unsigned int b2 = b1+1; b2 < players.size(); ++b2) {
                if(players[b2] == nullptr) {
                    continue;
                }

                for(unsigned int i = 0; i < botVsBotTimes; ++i) {
                    game::Game round(players[b1], players[b2]);
                    round.loadBoard(board);

                    unsigned int numOfMoves = 1;

                    while (!round.performNextMove().isGameOver()) { ++numOfMoves; }

                    game::GameState endState = round.getCurrentState();

                    m_statistics.addEntry({
                        board,
                        endState.m_board,
                        endState.m_status,
                        endState.m_player1->name(),
                        endState.m_player2->name(),
                        numOfMoves
                    });
                }
            }
        }
        return m_statistics;
    }

}