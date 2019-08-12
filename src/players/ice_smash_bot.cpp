#include "ice_smash_bot.h"
#include <algorithm>
#include <chrono>

namespace players {

    IceSmashBot::IceSmashBot() {
    }

    std::string IceSmashBot::name() const {
        return "SmashBot v1";
    }

    bool IceSmashBot::preferToStart(const game::Board& board) {
        return rand() % 2 == 0;
    }

    bool sortByScoreDescending(const IceSmashScore& a, const IceSmashScore& b) {
        return (b.first < a.first);
    }

    game::Move IceSmashBot::play(const game::Board& board) {
        auto allValidMoves = getAllValidMoves(board);
        auto allScores = getAllScores(board, allValidMoves);
        sort(allScores.begin(), allScores.end(), sortByScoreDescending);
        int maxScore = allScores[0].first;
        int numOfBestMoves = 1;
        for(; numOfBestMoves < allScores.size(); ++numOfBestMoves) {
            if(allScores[numOfBestMoves].first != maxScore) {
                break;
            }
        }
        auto randNum = rand() % numOfBestMoves;
        return allScores[randNum].second;
    }

    int IceSmashBot::findMoveAhead(const game::Board& board, unsigned int x, unsigned int y, int dir) const {
        int moveAhead = 0;
        unsigned int enemy = board[x][y] == 1 ? 2 : 1;
        for(unsigned int newY = y + dir; newY < 8; newY += dir) {
            if(board[x][newY]) {
                break;
            }
            unsigned int xNeg = x - 1;
            unsigned int xPos = x + 1;
            if(xNeg < 8 && board[xNeg][newY] == enemy) {
                break;
            }
            if(xPos < 8 && board[xPos][newY] == enemy) {
                break;
            }
            moveAhead += 1;
        }
        return moveAhead;
    }

    int IceSmashBot::getScore(const game::Board& board) const {
        int friendMoveAheadSum = 0;
        int enemyMoveAheadSum = 0;

        int friendDistanceScore = 0;
        int enemyDistanceScore = 0;

        for(unsigned int x = 0; x < 8; ++x) {
            for(unsigned int y = 0; y < 8; ++y) {
                if(board[x][y] == 1) {
                    friendMoveAheadSum += findMoveAhead(board, x, y, -1);
                    friendDistanceScore += (7 - y) * (7 - y);
                } else if(board[x][y] == 2) {
                    enemyMoveAheadSum += findMoveAhead(board, x, y, 1);
                    enemyDistanceScore += (y - 1) * (y - 1);
                }
            }
        }

        return friendMoveAheadSum + friendDistanceScore
             - enemyMoveAheadSum - enemyDistanceScore;
    }

    IceSmashScore IceSmashBot::getScoreIfMove(const game::Board& board, game::Move move) const {
        game::Board newBoard(board);
        move.performMove(newBoard);
        return IceSmashScore(getScore(newBoard), move);
    }

    std::vector<IceSmashScore> IceSmashBot::getAllScores(const game::Board& board, const std::vector<game::Move> moves) const {
        std::vector<IceSmashScore> result;

        for(unsigned int moveId = 0; moveId < moves.size(); ++moveId){
            result.push_back(getScoreIfMove(board, moves[moveId]));
        }

        return result;
    }
    
    std::vector<game::Move> IceSmashBot::getAllValidMoves(const game::Board& board) const {
        std::vector<game::Move> result;

        for(unsigned int x = 0; x < 8; ++x){
            for(unsigned int y = 0; y < 8; ++y){
                if(board[x][y] == 1) {
                    game::Move moveNeg (x,y,x-1,y-1);
                    game::Move moveZero(x,y,x+0,y-1);
                    game::Move movePos (x,y,x+1,y-1);

                    if(moveNeg.isValid(board, true)) {
                        result.push_back(moveNeg);
                    }

                    if(moveZero.isValid(board, true)) {
                        result.push_back(moveZero);
                    }

                    if(movePos.isValid(board, true)) {
                        result.push_back(movePos);
                    }
                }
            }
        }

        return result;
    }

}