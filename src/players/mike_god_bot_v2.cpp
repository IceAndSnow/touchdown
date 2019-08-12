#include "mike_god_bot_v2.h"
#include "mike.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#define MAX_NUM_MOVES 48
#define NUM_LEVELS 4

namespace players {

   /////////////////////////////////////////////////////////////
   // MikeGodBot

   MikeGodBotV2::MikeGodBotV2() {
      srand(time(0));
   }

   MikeGodBotV2::MikeGodBotV2(unsigned int seed) {
      srand(seed);
   }

   std::string MikeGodBotV2::name() const {
      return "God Bot v2";
   }

   bool MikeGodBotV2::preferToStart(const game::Board &board) {
      return rand() % 2 == 0; // Randomly choose to start or not
   }

   int MikeGodBotV2::eval(const MikeBoard& board) {
      int res = 0;
      const MikeHash& hash = board.getHash();

      if (hash.m_me & 0x00FF000000000000UL) {
         res = 1;
      } else if (hash.m_you & 0x00FF000000000000UL) {
         res = -1;
      } else if (hash.m_me & 0x0000FF0000000000UL) {
         res = 1;
      } else if (hash.m_you & 0x0000FF0000000000UL) {
         res = -1;
      }
//      std::cout << board;
//      std::cout << "Evaluation " << res << std::endl;
      return res;
   } // eval

   int MikeGodBotV2::search(MikeBoard board, float alpha, float beta, int level)
   {
      if (int res = board.getTurn() * board.isFinished()) {
         // If a player has won, then return.
         return res;
      }

      if (!level) {
         // If at leaf of tree, return the heuristic evaluation of the current position
         return eval(board);
      }

      int bestVal = -9999;
      MikeMove moves[MAX_NUM_MOVES];
      int moveCount = board.getLegalMoves(moves);

      if (!moveCount) {
         // If no legal moves, the game is a draw.
         return 0;
      }

      for (int i=0; i<moveCount && bestVal < beta; ++i) {
         if (bestVal > alpha) {
            alpha = bestVal;
         }
         board.makeMove(moves[i]);
         int val = -search(board, -beta, -alpha, level-1);
         board.undoMove();

         if (val > bestVal)
            bestVal = val;
      } // end of for

      return bestVal;
   } // end of search


   game::Move MikeGodBotV2::play(const game::Board &startBoard) {
      MikeBoard board(startBoard);     // current board
      MikeMove  moveList[MAX_NUM_MOVES];
      int       bestVal = -9999;
      MikeMove  bestMove;

      int moveCount = board.getLegalMoves(moveList);
      for (int i=0; i<moveCount; ++i) {
         int alpha = -9999;
         int beta = 9999;

         board.makeMove(moveList[i]);
         int val = -search(board, -beta, -alpha, NUM_LEVELS);
         board.undoMove();

         if (val > bestVal) {
            bestVal = val;
            bestMove = moveList[i];
         }

//         std::cout << "Move " << moveList[i] << " has value " << val << std::endl;
      } // end of for
//      std::cout << "Selected move " << bestMove << std::endl << std::endl;

      return game::Move(bestMove.col_from, 7-bestMove.row_from, bestMove.col_to, 7-bestMove.row_to);
   } // play

} // namespace players

