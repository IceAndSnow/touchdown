#include "mike_god_bot.h"
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

      uint64_t maskMeFrom  = 1UL << (move.row_from*8 + move.col_from);
      uint64_t maskMeTo    = 1UL << (move.row_to*8 + move.col_to);
      uint64_t maskYouFrom = 1UL << ((7-move.row_from)*8 + move.col_from);
      uint64_t maskYouTo   = 1UL << ((7-move.row_to)*8 + move.col_to);

      if (m_turn < 0) {
         std::swap(maskMeFrom, maskYouFrom);
         std::swap(maskMeTo, maskYouTo);
      }

      move.captured                         = m_board[move.col_to][move.row_to];
      m_board[move.col_to][move.row_to]     = m_board[move.col_from][move.row_from];
      m_board[move.col_from][move.row_from] = 0;
      m_turn = -m_turn;
      m_moveList.push_back(move);

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

      uint64_t maskMeFrom  = 1UL << (move.row_from*8 + move.col_from);
      uint64_t maskMeTo    = 1UL << (move.row_to*8 + move.col_to);
      uint64_t maskYouFrom = 1UL << ((7-move.row_from)*8 + move.col_from);
      uint64_t maskYouTo   = 1UL << ((7-move.row_to)*8 + move.col_to);

      if (m_turn > 0) {
         std::swap(maskMeFrom, maskYouFrom);
         std::swap(maskMeTo, maskYouTo);
      }

      m_board[move.col_from][move.row_from] = m_board[move.col_to][move.row_to];
      m_board[move.col_to][move.row_to]     = move.captured;
      m_turn = -m_turn;

      std::swap(m_hash.m_me, m_hash.m_you);

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

} // namespace players

