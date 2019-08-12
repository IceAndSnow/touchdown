#include "mike_bot.h"
#include "mike.h"
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

   std::ostream& operator<< (std::ostream& os, const MikeStats& arg) {
      return os << "[+" << arg.m_numWin << " =" << arg.m_numDraw << " -" << arg.m_numLoss << "]";
   } // MikeStats

   std::ostream& operator<< (std::ostream& os, const MikeResults& arg) {
      return os << arg.m_map.size();
   } // MikeResults

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
      for (unsigned int i=0; i<300; ++i) {
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

