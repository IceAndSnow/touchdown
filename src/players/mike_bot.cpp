#include "mike_bot.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>

namespace players {

   MikeBot::MikeBot() {
       srand(time(0));
   }

   MikeBot::MikeBot(unsigned int seed) {
       srand(seed);
   }

   std::string MikeBot::name() const {
       return "Mike Bot v1";
   }

   bool MikeBot::preferToStart(const game::Board &board) {
       return rand() % 2 == 0; // Randomly choose to start or not
   }

   MikeBoard::MikeBoard(const game::Board &board) {
      for(unsigned char y = 1; y < 8; ++y) {
         for(unsigned char x = 0; x < 8; ++x) {
            m_board[x][y] = board[x][y];
         }
      }
      m_turn = 1;
   } // MikeBoard

   void MikeBoard::makeMove(const MikeMove& _move) {
      MikeMove move(_move);   // Make a temporary copy
      move.captured                         = m_board[move.col_to][move.row_to];
      m_board[move.col_to][move.row_to]     = m_board[move.col_from][move.row_from];
      m_board[move.col_from][move.row_from] = 0;
      m_turn = -m_turn;
      m_moveList.push_back(move);

      m_hash.update(m_whiteToMove);
      if (m_turn == -1) {
         m_hash.update(m_white[move.col_from][move.row_from]);
         m_hash.update(m_white[move.col_to][move.row_to]);
         if (move.captured) {
            m_hash.update(m_black[move.col_to][move.row_to]);
         }
      } else {
         m_hash.update(m_black[move.col_from][move.row_from]);
         m_hash.update(m_black[move.col_to][move.row_to]);
         if (move.captured) {
            m_hash.update(m_white[move.col_to][move.row_to]);
         }
      }
   } // makeMove

   void MikeBoard::undoMove() {
      assert(m_moveList.size() > 0);

      MikeMove move(m_moveList.back());
      m_moveList.pop_back();
      m_board[move.col_from][move.row_from] = m_board[move.col_to][move.row_to];
      m_board[move.col_to][move.row_to]     = move.captured;
      m_turn = -m_turn;

      m_hash.update(m_whiteToMove);
      if (m_turn == 1) {
         m_hash.update(m_white[move.col_from][move.row_from]);
         m_hash.update(m_white[move.col_to][move.row_to]);
         if (move.captured) {
            m_hash.update(m_black[move.col_to][move.row_to]);
         }
      } else {
         m_hash.update(m_black[move.col_from][move.row_from]);
         m_hash.update(m_black[move.col_to][move.row_to]);
         if (move.captured) {
            m_hash.update(m_white[move.col_to][move.row_to]);
         }
      }
   } // undoMove

   int MikeBoard::isFinished() const {
      for (int col=0; col<8; ++col) {
         if (m_board[col][7] == 1)
            return 1;
         if (m_board[col][0] == -1)
            return -1;
      }

      return 0;
   } // isFinished

   int MikeBoard::getLegalMoves(MikeMove *pMove) const {
      int numMoves = 0;

      int firstRow;
      int lastRow;
      if (m_turn > 0) {
         firstRow = 0;
         lastRow = 6;
      }
      else {
         firstRow = 7;
         lastRow = 1;
      }

      // First promotion
      for (int col=0; col<8; ++col) {
         if (m_board[col][lastRow] == m_turn) {
            if (m_board[col][lastRow+m_turn] == 0) {
               pMove->captured = 0;
               pMove->col_from = col;
               pMove->row_from = lastRow;
               pMove->col_to   = col;
               pMove->row_to   = lastRow + m_turn;
               pMove++;
               numMoves++;
            }
            if (col > 0 && m_board[col-1][lastRow+m_turn] == -m_turn) {
               pMove->captured = -m_turn;
               pMove->col_from = col;
               pMove->row_from = lastRow;
               pMove->col_to   = col - 1;
               pMove->row_to   = lastRow + m_turn;
               pMove++;
               numMoves++;
            }
            if (col < 7 && m_board[col+1][lastRow+m_turn] == -m_turn) {
               pMove->captured = -m_turn;
               pMove->col_from = col;
               pMove->row_from = lastRow;
               pMove->col_to   = col + 1;
               pMove->row_to   = lastRow + m_turn;
               pMove++;
               numMoves++;
            }
         }
      }

      // Then captures from the last row
      {
         int row=lastRow;
         for (int col=0; col<8; ++col) {
            if (m_board[col][row] == m_turn) {
               if (col > 0 && m_board[col-1][row+m_turn] == -m_turn) {
                  pMove->captured = -m_turn;
                  pMove->col_from = col;
                  pMove->row_from = row;
                  pMove->col_to   = col - 1;
                  pMove->row_to   = row + m_turn;
                  pMove++;
                  numMoves++;
               }
               if (col < 7 && m_board[col+1][row+m_turn] == -m_turn) {
                  pMove->captured = -m_turn;
                  pMove->col_from = col;
                  pMove->row_from = row;
                  pMove->col_to   = col + 1;
                  pMove->row_to   = row + m_turn;
                  pMove++;
                  numMoves++;
               }
            }
         }
      }

      // Then captures from the first row
      {
         int row=firstRow;
         for (int col=0; col<8; ++col) {
            if (m_board[col][row] == m_turn) {
               bool decisive = false;
               if (col > 0 && m_board[col-1][row+m_turn] == -m_turn) {
                  pMove->captured = -m_turn;
                  pMove->col_from = col;
                  pMove->row_from = row;
                  pMove->col_to   = col - 1;
                  pMove->row_to   = row + m_turn;
                  pMove++;
                  numMoves++;
                  if (m_board[col-1][row] == m_turn) {
                     decisive = true;
                  }
               }
               if (col < 7 && m_board[col+1][row+m_turn] == -m_turn) {
                  pMove->captured = -m_turn;
                  pMove->col_from = col;
                  pMove->row_from = row;
                  pMove->col_to   = col + 1;
                  pMove->row_to   = row + m_turn;
                  pMove++;
                  numMoves++;
                  if (m_board[col+1][row] == m_turn) {
                     decisive = true;
                  }
               }
               if (decisive &&
                     m_board[col][row+m_turn] == 0) {
                  pMove->captured = 0;
                  pMove->col_from = col;
                  pMove->row_from = row;
                  pMove->col_to   = col;
                  pMove->row_to   = row + m_turn;
                  pMove++;
                  numMoves++;
               }
            }
         }
      }

      // Then captures from elsewhere
      for (int row=firstRow+m_turn; row!=lastRow; row+=m_turn) {
         for (int col=0; col<8; ++col) {
            if (m_board[col][row] == m_turn) {
               if (col > 0 && m_board[col-1][row+m_turn] == -m_turn) {
                  pMove->captured = -m_turn;
                  pMove->col_from = col;
                  pMove->row_from = row;
                  pMove->col_to   = col - 1;
                  pMove->row_to   = row + m_turn;
                  pMove++;
                  numMoves++;
               }
               if (col < 7 && m_board[col+1][row+m_turn] == -m_turn) {
                  pMove->captured = -m_turn;
                  pMove->col_from = col;
                  pMove->row_from = row;
                  pMove->col_to   = col + 1;
                  pMove->row_to   = row + m_turn;
                  pMove++;
                  numMoves++;
               }
            }
         }
      }

      // Then ordinary moves where no opponent is opposing
      for (int col=0; col<8; ++col) {
         int row;
         // Search for piece farthest away
         for (row=lastRow+m_turn; row!=firstRow-m_turn; row-=m_turn) {
            if (m_board[col][row])
               break; // Found a piece
         }
         if (row!=firstRow-m_turn // There is a piece
               && m_board[col][row] == m_turn // It is ours
               && row!=lastRow) { // We are not promoting (have already taken care of that)
            for (; row!=firstRow-m_turn; row-=m_turn) {
               if (m_board[col][row] == m_turn) {
                  if (m_board[col][row+m_turn] == 0) {
                     pMove->captured = 0;
                     pMove->col_from = col;
                     pMove->row_from = row;
                     pMove->col_to   = col;
                     pMove->row_to   = row + m_turn;
                     pMove++;
                     numMoves++;
                  }
               }
            }
         }
      }

      // Then ordinary moves
      for (int col=0; col<8; ++col) {
         int row;
         // Search for piece farthest away
         for (row=lastRow+m_turn; row!=firstRow-m_turn; row-=m_turn) {
            if (m_board[col][row])
               break; // Found a piece
         }
         if (!(row!=firstRow-m_turn && m_board[col][row] == m_turn)) {
            for (; row!=firstRow-m_turn; row-=m_turn) {
               if (m_board[col][row] == m_turn) {
                  if (m_board[col][row+m_turn] == 0) {
                     pMove->captured = 0;
                     pMove->col_from = col;
                     pMove->row_from = row;
                     pMove->col_to   = col;
                     pMove->row_to   = row + m_turn;
                     pMove++;
                     numMoves++;
                  }
               }
            }
         }
      }

      return numMoves;
   } // getLegalMoves

   MikeHash MikeBoard::getHash(const MikeMove& move) const {
      MikeBoard *pBoard = (MikeBoard *) this;
      pBoard->makeMove(move);
      MikeHash res = pBoard->getHash();
      pBoard->undoMove();
      return res;
   } // getHash

   const MikeMove& MikeBot::getRandomMove(const MikeBoard& board, const MikeMove *moveList, int moveCount) const {
      float probs[48];
      float probTot = 0.0;

      for (int i=0; i<moveCount; ++i) {
         float val = -m_results[board.getHash(moveList[i])].getResult();
         float prob = (1.05 + val) / (1.05 - val);
         probTot += prob;
         probs[i] = prob;
      }

      float uniform = rand() / (RAND_MAX + 1.0);
      float search = probTot * uniform;
      float val = 0.0;
      for (int i=0; i<moveCount; ++i) {
         val += probs[i];
         if (val >= search) {
            return moveList[i];
         }
      }

      // Should never get here!
      assert(false);
   } // getRandomMove


   // This plays a single game to the end, making random moves for both sides.
   int MikeBot::playGameToEnd(const MikeBoard &startBoard) {
      MikeBoard    board(startBoard);     // make temporary copy.
      int          res;                   // result to return.
      MikeHash     hash(board.getHash()); // current hash.

      while (true) {
         res = board.isFinished();
         if (res) {
            // If there is a winner, return.
            break;
         }

         MikeMove moveList[48];              // list of legal moves in current position

         int moveCount = board.getLegalMoves(moveList);
         if (!moveCount) {
            // If there is no winner, and no legal moves,
            // the game is a draw.
            break;
         }

         MikeMove move(getRandomMove(board, moveList, moveCount));

         board.makeMove(move);
      } // end of while(true)

      while (true) {
         m_results.update(board.getHash(), res * board.getTurn());
         if (board.getHash() == hash)
            break;

         board.undoMove();
      } // end of while(true)

      return res;
   } // playGameToEnd

   float MikeBot::search(const MikeBoard& board) {
      // Play a bunch of random games
      for (unsigned int i=0; i<100; ++i) {
         playGameToEnd(board);
      }

      // Get the results of the current position.
      return m_results[board.getHash()].getResult();
   } // search

   game::Move MikeBot::play(const game::Board &startBoard) {
      MikeBoard board(startBoard);     // current board
      MikeMove  moveList[48];
      float     bestVal = -2.0;
      MikeMove  bestMove;

      int moveCount = board.getLegalMoves(moveList);
      for (int i=0; i<moveCount; ++i) {
         board.makeMove(moveList[i]);
         float val = -search(board);
         board.undoMove();

         if (val > bestVal) {
            bestVal = val;
            bestMove = moveList[i];
         }
      } // end of for

      return game::Move(bestMove.col_from, bestMove.row_from, bestMove.col_to, bestMove.row_to);
   } // play

   // Negation operator
   MikeStats operator-(const MikeStats& rhs) {
      MikeStats res;
      res.m_numWin  = rhs.m_numLoss;
      res.m_numDraw = rhs.m_numDraw;
      res.m_numLoss = rhs.m_numWin;

      return res;
   } // operator-

} // namespace players

