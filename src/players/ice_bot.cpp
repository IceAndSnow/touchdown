#include "ice_bot.h"

namespace players {

    static inline game::Move arbitraryValidMove(const game::Board& board) {
        for(unsigned char x = 0; x < 8; ++x) {
            for(unsigned char y = 0; y < 8; ++y) {
                if(board[x][y] == 1) { //If this is the current player's tile
                    if(board[x][y-1] == 0) {
                        return game::Move(x, y, x, y-1);
                    }
                    if(x>0 && board[x-1][y-1] == 2) {
                        return game::Move(x, y, x-1, y-1);
                    }
                    if(x<7 && board[x+1][y-1] == 2) {
                        return game::Move(x, y, x+1, y-1);
                    }
                }
            }
        }
        return game::Move(0,0,1,1); //Should not happen
    }

    std::string IceBot::name() {
        return "IceBot v0.1";
    }

    game::Move IceBot::play(const game::Board &board) {
        return arbitraryValidMove(board);
    }

    bool IceBot::preferToStart(const game::Board &board) {
        return true;
    }

}