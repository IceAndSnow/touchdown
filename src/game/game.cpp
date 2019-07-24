#include "game.h"

namespace game {

    Game::Game(game::Player *p1, game::Player *p2) {
        m_state.m_board.initStandard();
        m_state.m_turn = true;
        m_state.m_winner = nullptr;
        m_state.m_status = NOT_YET_DECIDED;
        bool p1WantsToStart = p1->preferToStart(m_state.m_board);
        bool p2WantsToStart = p2->preferToStart(m_state.m_board);
        if(p1WantsToStart == p2WantsToStart){
            if(rand() % 2 == 0) {
                m_state.m_player1 = p1;
                m_state.m_player2 = p2;
            } else {
                m_state.m_player1 = p2;
                m_state.m_player2 = p1;
            }
        } else if(p1WantsToStart) {
            m_state.m_player1 = p1;
            m_state.m_player2 = p2;
        } else {
            m_state.m_player1 = p2;
            m_state.m_player2 = p1;
        }
    }

    void Game::loadBoard(game::Board board) {
        m_state.m_board = board;
    }

    static Move invertMove(Move move) {
        return Move(move.m_fromX, 7-move.m_fromY, move.m_toX, 7-move.m_toY);
    }

    static Board invertBoard(const Board& board) {
        Board newBoard;
        for(unsigned char x = 0; x < 8; ++x) {
            for(unsigned char y = 0; y < 8; ++y) {
                if(board[x][7-y] == 1) {
                    newBoard[x][y] = 2;
                } else if(board[x][7-y] == 2) {
                    newBoard[x][y] = 1;
                } else {
                    newBoard[x][y] = 0;
                }
            }
        }
        return newBoard;
    }

    static inline bool hasPlayer1Won(const Board& board) {
        //Winning condition for Player 1:
        for(unsigned char i = 0; i < 8; ++i) {
            if(board[i][0] == 1) {
                return true;
            }
        }
        return false;
    }

    static inline bool hasPlayer2Won(const Board& board) {
        //Winning condition for Player 2:
        for(unsigned char i = 0; i < 8; ++i) {
            if(board[i][7] == 2) {
                return true;
            }
        }
        return false;
    }

    static inline bool hasPlayer1AnyValidMoves(const Board& board) {
        for(unsigned char x = 0; x < 8; ++x) {
            for(unsigned char y = 0; y < 8; ++y) {
                if(board[x][y] == 1) {
                    if(board[x][y-1] == 0) {
                        return true;
                    }
                    if(x>0 && board[x-1][y-1] == 2) {
                        return true;
                    }
                    if(x<7 && board[x+1][y-1] == 2) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    static inline bool hasPlayer2AnyValidMoves(const Board& board) {
        for(unsigned char x = 0; x < 8; ++x) {
            for(unsigned char y = 0; y < 8; ++y) {
                if(board[x][y] == 2) {
                    if(board[x][y+1] == 0) {
                        return true;
                    }
                    if(x>0 && board[x-1][y+1] == 1) {
                        return true;
                    }
                    if(x<7 && board[x+1][y+1] == 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    static unsigned char calculateWinner(const Board& board, bool turn) {
        if(hasPlayer1Won(board)) {
            return PLAYER_1;
        }
        if(hasPlayer2Won(board)) {
            return PLAYER_2;
        }
        if(turn && !hasPlayer1AnyValidMoves(board)) {
            return TIE;
        }
        if(!turn && !hasPlayer2AnyValidMoves(board)) {
            return TIE;
        }
        return NOT_YET_DECIDED;
    }

    GameState Game::performNextMove() {
        if(m_state.isGameOver()) {
            return m_state;
        }

        const Move move = m_state.m_turn ?
                m_state.m_player1->play(m_state.m_board) :
                invertMove(m_state.m_player2->play(invertBoard(m_state.m_board)));

        if(!move.isValid(m_state.m_board, m_state.m_turn)) {
            m_state.m_status = INVALID_MOVE_PERFORMED;
            return m_state;
        }
        move.performMove(m_state.m_board);

        m_state.m_turn = !m_state.m_turn;

        unsigned char winner = calculateWinner(m_state.m_board, m_state.m_turn);
        if(winner != NOT_YET_DECIDED) {
            m_state.m_status = winner;
            m_state.m_winner = winner == PLAYER_1 ? m_state.m_player1 :
                              (winner == PLAYER_2 ? m_state.m_player2 : nullptr);
        }
        return m_state;
    }

    GameState Game::getCurrentState() {
        return m_state;
    }

}