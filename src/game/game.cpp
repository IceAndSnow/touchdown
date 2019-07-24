#include "game.h"

namespace game {

    Game::Game(game::Player *p1, game::Player *p2) {
        m_board.initStandard();
        m_turn = true;
        bool p1WantsToStart = p1->preferToStart(m_board);
        bool p2WantsToStart = p2->preferToStart(m_board);
        if(p1WantsToStart == p2WantsToStart){
            if(rand() % 2 == 0) {
                m_players[0] = p1;
                m_players[1] = p2;
            } else {
                m_players[0] = p2;
                m_players[1] = p1;
            }
        } else if(p1WantsToStart) {
            m_players[0] = p1;
            m_players[1] = p2;
        } else {
            m_players[0] = p2;
            m_players[1] = p1;
        }
    }

    void Game::loadBoard(game::Board board) {
        m_board = board;
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
            return PLAYER_2;
        }
        if(!turn && !hasPlayer2AnyValidMoves(board)) {
            return PLAYER_1;
        }
        return NOT_YET_DECIDED;
    }

    GameState Game::performNextMove() {
        unsigned char winner = calculateWinner(m_board, m_turn);
        if(winner != NOT_YET_DECIDED) {
            return {
                m_board,
                m_turn,
                winner,
                winner == PLAYER_1 ? m_players[0] : m_players[1],
                m_players[0],
                m_players[1]
            };
        }

        const Move move = m_turn ?
                m_players[0]->play(m_board) :
                invertMove(m_players[1]->play(invertBoard(m_board)));

        if(!move.isValid(m_board, m_turn)) {
            return {
                    m_board,
                    m_turn,
                    INVALID_MOVE_PERFORMED,
                    nullptr
            };
        }
        move.performMove(m_board);

        m_turn = !m_turn;

        winner = calculateWinner(m_board, m_turn);
        return {
            m_board,
            m_turn,
            winner,
            winner == PLAYER_1 ? m_players[0] : m_players[1],
            m_players[0],
            m_players[1]
        };
    }

}