#include "ice_bot.h"
#include <cstdlib>
#include <vector>
#include <string.h>
#include <iostream>
#include "../grundy/grundy.h"

namespace players {

    static inline game::Move arbitraryValidMove(const game::Board& board) {
        for(unsigned char y = 0; y < 8; ++y) {
            for(unsigned char x = 0; x < 8; ++x) {
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

    typedef std::pair<unsigned char, unsigned char> pos;

    static std::vector<pos> getOwnPawnsSorted(const game::Board& board) {
        std::vector<pos> result;
        for(unsigned char y = 1; y < 7; ++y) {
            for(unsigned char x = 0; x < 8; ++x) {
                if(board[x][y] == 1) {
                    result.push_back({ x, y });
                }
            }
        }
        return result;
    }

    static unsigned int partialGrundy(const game::Board& board, bool turn, pos p) {
        unsigned char &px = p.first;
        unsigned char &py = p.second;
        char tiles[8][8];

        if(px == 0) {
            px = 1;
        }
        if(px == 7) {
            px = 6;
        }

        for(unsigned char x = 0; x < 8; ++x) {
            for(unsigned char y = 0; y < 8; ++y) {
                if(x < 3) {
                    tiles[x][y] = board[(char)px+(char)x-1][y];
                } else {
                    tiles[x][y] = 0;
                }
            }
        }

        grundy::GrundyBoardState state;
        memcpy(state.m_board, tiles, sizeof(tiles));
        state.m_turn = turn;

        return grundy::grundy(state);
    }

    static game::Move getOptimalPartialGrundyMove(game::Board board, pos p) {

        unsigned char &x = p.first;
        unsigned char &y = p.second;

        game::Move straight(x, y, x, y-1);

        if(straight.isValid(board, true)) {
            board[x][y] = 0;
            board[x][y-1] = 1;
            if(partialGrundy(board, false, { x, y-1 }) == 0) {
                return straight;
            }
            board[x][y] = 1;
            board[x][y-1] = 0;
        }
        char dx;
        if(x < 4) {
            dx = -1;
        } else {
            dx = 1;
        }

        game::Move diag_good(x, y, x+dx, y-1);
        game::Move diag_bad(x, y, x-dx, y-1);

        if(diag_good.isValid(board, true)) {
            board[x][y] = 0;
            board[x+dx][y-1] = 1;
            if(partialGrundy(board, false, { x+dx, y-1 }) == 0) {
                return diag_good;
            }
            board[x][y] = 1;
            board[x+dx][y-1] = 2;
        }

        if(diag_bad.isValid(board, true)) {
            board[x][y] = 0;
            board[x-dx][y-1] = 1;
            if(partialGrundy(board, false, { x-dx, y-1 }) == 0) {
                return diag_bad;
            }
            board[x][y] = 1;
            board[x-dx][y-1] = 2;
        }

        if(diag_good.isValid(board, true)) {
            board[x][y] = 0;
            board[x+dx][y-1] = 1;
            if(partialGrundy(board, false, { x, y-1 }) == 0) {
                return diag_good;
            }
            board[x][y] = 1;
            board[x+dx][y-1] = 2;
        }

        if(diag_bad.isValid(board, true)) {
            board[x][y] = 0;
            board[x-dx][y-1] = 1;
            if(partialGrundy(board, false, { x, y-1 }) == 0) {
                return diag_bad;
            }
            board[x][y] = 1;
            board[x-dx][y-1] = 2;
        }

        return game::Move(-1, -1, -1, -1);
    }

    static game::Move partialGrundyAlgorithm(const game::Board& board) {
        std::vector<pos> positions = getOwnPawnsSorted(board);
        for(pos& p : positions) {
            if(partialGrundy(board, true, p) != 0) {
                game::Move move = getOptimalPartialGrundyMove(board, p);
                if(move.isValid(board, true)){
                    return move;
                }
            }
        }
        return arbitraryValidMove(board);
    }

    std::string IceBot::name() const {
        return "IceBot v1.0";
    }

    game::Move IceBot::play(const game::Board &board) {
        return partialGrundyAlgorithm(board);
    }

    bool IceBot::preferToStart(const game::Board &board) {
        return true;
    }

}