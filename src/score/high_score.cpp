#include "high_score.h"
#include <ctime>


namespace score {

    void HighScore::printStatistics() {
        m_statistics.print();
    }

    void HighScore::resetStatistics() {
        m_statistics.clear();
    }

    Statistics HighScore::recordTournament(std::vector<players::PlayerInstantiator*> playerGens, unsigned int botVsBotTimes) {
        game::Board defaultBoard;
        defaultBoard.initStandard();
        return recordTournament(playerGens, defaultBoard, botVsBotTimes);
    }

    Statistics HighScore::recordTournament(std::vector<players::PlayerInstantiator*> playerGens, game::Board board, unsigned int botVsBotTimes) {
        for(unsigned int b1 = 0; b1 < playerGens.size(); ++b1) {
            if(playerGens[b1] == nullptr) {
                continue;
            }
            for(unsigned int b2 = b1+1; b2 < playerGens.size(); ++b2) {
                if(playerGens[b2] == nullptr) {
                    continue;
                }

                for(unsigned int i = 0; i < botVsBotTimes; ++i) {
                    game::Game round(playerGens[b1]->createNewPlayer(), playerGens[b2]->createNewPlayer());
                    round.loadBoard(board);

                    unsigned int numOfMoves = 1;

                    unsigned int player1TotalTime = 0;
                    unsigned int player2TotalTime = 0;

                    struct timespec sTime, eTime;
                    clock_gettime(CLOCK_REALTIME, &sTime);
                    while (!round.performNextMove().isGameOver()) {
                        clock_gettime(CLOCK_REALTIME, &eTime);
                        if(eTime.tv_nsec < sTime.tv_nsec) {
                            eTime.tv_nsec += 1000000000;
                            eTime.tv_sec -= 1;
                        }
                        unsigned int elapsedTime = (eTime.tv_sec - sTime.tv_sec) * 1000000000
                                                 + (eTime.tv_nsec - sTime.tv_nsec);
                        if(numOfMoves % 2 == 0) {
                            player2TotalTime += elapsedTime;
                        } else {
                            player1TotalTime += elapsedTime;
                        }
                        ++numOfMoves;
                        clock_gettime(CLOCK_REALTIME, &sTime);
                    }

                    game::GameState endState = round.getCurrentState();

                    m_statistics.addEntry({
                        board,
                        endState.m_board,
                        endState.m_status,
                        endState.m_player1->name(),
                        endState.m_player2->name(),
                        numOfMoves,
                        player1TotalTime / 1000000,
                        player2TotalTime / 1000000
                    });
                    playerGens[b1]->cleanUp();
                    playerGens[b2]->cleanUp();
                }
            }
        }
        return m_statistics;
    }

}