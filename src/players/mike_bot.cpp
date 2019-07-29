#include "mike_bot.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace players {
   /////////////////////////////////////////////////////////////
   // Debug output

   std::ostream& operator<< (std::ostream& os, const MikeHash& arg) {
      return os << std::setw(16) << std::setfill('0') << std::hex << arg.m_me << "."
                << std::setw(16) << std::setfill('0') << std::hex << arg.m_you << std::dec;
   } // MikeHash

   std::ostream& operator<< (std::ostream& os, const MikeStats& arg) {
      return os << "[+" << arg.m_numWin << " =" << arg.m_numDraw << " -" << arg.m_numLoss << "]";
   } // MikeStats

   std::ostream& operator<< (std::ostream& os, const MikeResults& arg) {
      return os << arg.m_map.size();
   } // MikeResults

   std::ostream& operator<< (std::ostream& os, const MikeMove& arg) {
      char str[6];
      str[0] = arg.col_from + 'A';
      str[1] = arg.row_from + '1';
      str[2] = "x-x" [1+arg.captured];
      str[3] = arg.col_to + 'A';
      str[4] = arg.row_to + '1';
      str[5] = 0;

      return os << str;
   } // MikeMove

   std::ostream& operator<< (std::ostream& os, const MikeBoard& arg) {
      for (int row=7; row>=0; --row) {
         for (int col=0; col<8; ++col) {
            os << "O.X" [1+arg.m_board[col][row]];
         }
         os << std::endl;
      }
      os << "O.X"[1+arg.m_turn] << " to move." << std::endl;
      os << "Hash = " << arg.m_hash << std::endl;
      return os;
   } // MikeBoard


   /////////////////////////////////////////////////////////////
   // MikeStats

   // Negation operator
   MikeStats operator-(const MikeStats& rhs) {
      MikeStats res;
      res.m_numWin  = rhs.m_numLoss;
      res.m_numDraw = rhs.m_numDraw;
      res.m_numLoss = rhs.m_numWin;

      return res;
   } // operator-


   /////////////////////////////////////////////////////////////
   // MikeBoard

   MikeBoard::MikeBoard(const game::Board &board) {
      for(unsigned char y = 0; y < 8; ++y) {
         for(unsigned char x = 0; x < 8; ++x) {
            m_board[x][y] = ((1+board[x][7-y])%3)-1;
            switch (m_board[x][y]) {
               case 1  : m_hash.m_me |= 1UL << (y*8+x); break;
               case -1 : m_hash.m_you |= 1UL << ((7-y)*8+x); break;
            } 
         }
      }
      m_turn = 1;
   } // MikeBoard

   void MikeBoard::makeMove(const MikeMove& _move) {
      MikeMove move(_move);   // Make a temporary copy

      uint64_t maskMeFrom  = 1UL << (m_turn > 0 ? (move.row_from*8 + move.col_from)     : ((7-move.row_from)*8 + move.col_from));
      uint64_t maskMeTo    = 1UL << (m_turn > 0 ? (move.row_to*8 + move.col_to)         : ((7-move.row_to)*8 + move.col_to));
      uint64_t maskYouFrom = 1UL << (m_turn > 0 ? ((7-move.row_from)*8 + move.col_from) : (move.row_from*8 + move.col_from));
      uint64_t maskYouTo   = 1UL << (m_turn > 0 ? ((7-move.row_to)*8 + move.col_to)     : (move.row_to*8 + move.col_to));

      move.captured                         = m_board[move.col_to][move.row_to];
      m_board[move.col_to][move.row_to]     = m_board[move.col_from][move.row_from];
      m_board[move.col_from][move.row_from] = 0;
      m_turn = -m_turn;
      m_moveList.push_back(move);

      assert ((m_hash.m_me & maskMeFrom) != 0);
      assert ((m_hash.m_me & maskMeTo) == 0);
      assert ((m_hash.m_you & maskYouFrom) == 0);
      if (move.captured) {
         assert ((m_hash.m_you & maskYouTo) != 0);
      } else {
         assert ((m_hash.m_you & maskYouTo) == 0);
      }

      m_hash.m_me &= ~maskMeFrom;
      m_hash.m_me |= maskMeTo;
      if (move.captured) {
         m_hash.m_you &= ~maskYouTo;
      }

      std::swap(m_hash.m_me, m_hash.m_you);
   } // makeMove

   void MikeBoard::undoMove() {
      assert(m_moveList.size() > 0);
      MikeMove move(m_moveList.back());
      m_moveList.pop_back();

      uint64_t maskMeFrom  = 1UL << (m_turn < 0 ? (move.row_from*8 + move.col_from)     : ((7-move.row_from)*8 + move.col_from));
      uint64_t maskMeTo    = 1UL << (m_turn < 0 ? (move.row_to*8 + move.col_to)         : ((7-move.row_to)*8 + move.col_to));
      uint64_t maskYouFrom = 1UL << (m_turn < 0 ? ((7-move.row_from)*8 + move.col_from) : (move.row_from*8 + move.col_from));
      uint64_t maskYouTo   = 1UL << (m_turn < 0 ? ((7-move.row_to)*8 + move.col_to)     : (move.row_to*8 + move.col_to));

      m_board[move.col_from][move.row_from] = m_board[move.col_to][move.row_to];
      m_board[move.col_to][move.row_to]     = move.captured;
      m_turn = -m_turn;

      std::swap(m_hash.m_me, m_hash.m_you);

      assert ((m_hash.m_me & maskMeFrom) == 0);
      assert ((m_hash.m_me & maskMeTo) != 0);
      assert ((m_hash.m_you & maskYouFrom) == 0);
      assert ((m_hash.m_you & maskYouTo) == 0);

      m_hash.m_me |= maskMeFrom;
      m_hash.m_me &= ~maskMeTo;
      if (move.captured) {
         m_hash.m_you |= maskYouTo;
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
         firstRow = 1;
         lastRow = 7;
      }
      else {
         firstRow = 6;
         lastRow = 0;
      }

      for (int row=firstRow; row!=lastRow; row+=m_turn) {
         for (int col=0; col<8; ++col) {
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

      return numMoves;
   } // getLegalMoves

   MikeHash MikeBoard::getHash(const MikeMove& move) const {
      MikeBoard *pBoard = (MikeBoard *) this;
      pBoard->makeMove(move);
      MikeHash res(pBoard->getHash());
      pBoard->undoMove();
      return res;
   } // getHash


   /////////////////////////////////////////////////////////////
   // MikeBot

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

   // Select a random move using a probability distribution based on the result statistics.
   const MikeMove& MikeBot::getRandomMove(const MikeBoard& board, const MikeMove *moveList, int moveCount) const {
      float probs[48];
      float probTot = 0.0;

      for (int i=0; i<moveCount; ++i) {
         MikeStats stats = -m_results[board.getHash(moveList[i])];
         float prob = stats.m_numWin + 10.0;
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
   int MikeBot::playGameToEnd(const MikeBoard &startBoard, int num) {
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

   game::Move MikeBot::play(const game::Board &startBoard) {
      MikeBoard board(startBoard);     // current board
      MikeMove  moveList[48];
      float     bestVal = -2.0;
      MikeMove  bestMove;

      // Play a bunch of random games
      for (unsigned int i=0; i<100; ++i) {
         playGameToEnd(board, i);
      }

      int moveCount = board.getLegalMoves(moveList);
      for (int i=0; i<moveCount; ++i) {
         board.makeMove(moveList[i]);
         MikeStats stats = -m_results[board.getHash()];
         float val = stats.getResult();
         board.undoMove();

         if (val > bestVal || (val == bestVal && (rand() % 2) == 0)) {
            bestVal = val;
            bestMove = moveList[i];
         }
      } // end of for

      // Delete seldom used entries in the map, to make the map much smaller, and thereby faster.
      m_results.trim(10);

      return game::Move(bestMove.col_from, 7-bestMove.row_from, bestMove.col_to, 7-bestMove.row_to);
   } // play

} // namespace players

