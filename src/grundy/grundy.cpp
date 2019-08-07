#include "grundy.h"
#include <iostream>

namespace grundy {

    bool GrundyBoardState::operator==(const grundy::GrundyBoardState &b) const {
        for(unsigned char x = 0; x < 8; ++x) {
            for(unsigned char y = 0; y < 8; ++y) {
                if(m_board[x][y] != b.m_board[x][y]) {
                    return false;
                }
            }
        }

        return m_turn == b.m_turn;
    }

    std::size_t GrundyBoardStateHasher::operator()(const grundy::GrundyBoardState &b) const {
        std::size_t hash = 0;
        for(unsigned int i = 0; i < sizeof(b.m_board) / sizeof(int); ++i){
            hash ^= std::hash<int>{}(((int*)&b)[i]);
        }
        return hash ^ b.m_turn;
    }

    unsigned int Grundy::mex(const std::unordered_set<unsigned int> set) {
        for(unsigned int i = 0; i < set.size(); ++i) {
            if(set.find(i) == set.end()) {
                return i;
            }
        }
        return set.size();
    }

    inline unsigned int Grundy::grundyForCell(
            std::unordered_set<unsigned int>& set,
            GrundyBoardState& state,
            const unsigned char x,
            const unsigned char y) {

        // First make sure that only the current player's pieces can be moved in this turn

        if(state.m_board[x][y] == 1 && state.m_turn) {
            state.m_board[x][y] = 0;
            if(state.m_board[x][y-1] == 0) {
                // If this move is allowed, try performing the move
                // and calculate that states grundy number
                // and save it in the set
                state.m_board[x][y-1] = 1;
                state.m_turn = !state.m_turn;
                set.insert(grundy(state));
                // remember to reset the board after the move
                state.m_turn = !state.m_turn;
                state.m_board[x][y-1] = 0;
            }
            if(x > 0 && state.m_board[x-1][y-1] == 2) {
                // If this move is allowed, try performing the move
                // and calculate that states grundy number
                // and save it in the set
                state.m_board[x-1][y-1] = 1;
                state.m_turn = !state.m_turn;
                set.insert(grundy(state));
                // remember to reset the board after the move
                state.m_turn = !state.m_turn;
                state.m_board[x-1][y-1] = 2;
            }
            if(x < 7 && state.m_board[x+1][y-1] == 2) {
                // If this move is allowed, try performing the move
                // and calculate that states grundy number
                // and save it in the set
                state.m_board[x+1][y-1] = 1;
                state.m_turn = !state.m_turn;
                set.insert(grundy(state));
                // remember to reset the board after the move
                state.m_turn = !state.m_turn;
                state.m_board[x+1][y-1] = 2;
            }
            state.m_board[x][y] = 1;
        } else if(state.m_board[x][y] == 2 && !state.m_turn) {
            state.m_board[x][y] = 0;
            if(state.m_board[x][y+1] == 0) {
                // If this move is allowed, try performing the move
                // and calculate that states grundy number
                // and save it in the set
                state.m_board[x][y+1] = 2;
                state.m_turn = !state.m_turn;
                set.insert(grundy(state));
                // remember to reset the board after the move
                state.m_turn = !state.m_turn;
                state.m_board[x][y+1] = 0;
            }
            if(x > 0 && state.m_board[x-1][y+1] == 1) {
                // If this move is allowed, try performing the move
                // and calculate that states grundy number
                // and save it in the set
                state.m_board[x-1][y+1] = 2;
                state.m_turn = !state.m_turn;
                set.insert(grundy(state));
                // remember to reset the board after the move
                state.m_turn = !state.m_turn;
                state.m_board[x-1][y+1] = 1;
            }
            if(x < 7 && state.m_board[x+1][y+1] == 1) {
                // If this move is allowed, try performing the move
                // and calculate that states grundy number
                // and save it in the set
                state.m_board[x+1][y+1] = 2;
                state.m_turn = !state.m_turn;
                set.insert(grundy(state));
                // remember to reset the board after the move
                state.m_turn = !state.m_turn;
                state.m_board[x+1][y+1] = 1;
            }
            state.m_board[x][y] = 2;
        }
    }

    static inline bool hasPlayer1Won(const GrundyBoardState& state) {
        //Winning condition for Player 1:
        for(unsigned char i = 0; i < 8; ++i) {
            if(state.m_board[i][0] == 1) {
                return true;
            }
        }
        return false;
    }

    static inline bool hasPlayer2Won(const GrundyBoardState& state) {
        //Winning condition for Player 2:
        for(unsigned char i = 0; i < 8; ++i) {
            if(state.m_board[i][7] == 2) {
                return true;
            }
        }
        return false;
    }

    inline unsigned int Grundy::grundyNoMemo(GrundyBoardState& state) {
        if(state.m_turn) {
            // Since it is currently Player 1's turn,
            // if Player 2 has won, then the grundy number
            // must be 0, as this state is then lost.
            if(hasPlayer2Won(state)) {
                return 0;
            }
        } else {
            // Since it is currently Player 2's turn,
            // if Player 1 has won, then the grundy number
            // must be 0, as this state is then lost.
            if(hasPlayer1Won(state)) {
                return 0;
            }
        }

        std::unordered_set<unsigned int> set;
        for(unsigned char x = 0; x < 8; ++x) {
            for(unsigned char y = 1; y < 7; ++y) {
                // Add all grundy numbers achieved by moving the piece on (x,y) to the set.
                grundyForCell(set, state, x, y);
            }
        }

        // Calculate the grundy number for the state
        // by using the mex-function
        return mex(set);
    }

    unsigned int Grundy::grundy(GrundyBoardState& state) {
        //Dynamic Programming, as many states otherwise would be calculated many times
        auto it = memo.find(state);
        if(it != memo.end()) {
            //If it has previously been calculated, simply return the last answer:
            return it->second;
        }
        //If the answer has not been calculated, calculate it, save it and return it:
        unsigned int result = grundyNoMemo(state);
        memo.insert({state, result});
        return result;
    }

}