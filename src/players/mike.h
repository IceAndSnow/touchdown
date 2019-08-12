#ifndef TOUCHDOWN_MIKE_H
#define TOUCHDOWN_MIKE_H

#include "../game/player.h"
#include <vector>
#include <map>
#include <iostream>

namespace players {

   class MikeHash {
      public:
         MikeHash() : m_me(0), m_you(0) {}
         void update(const MikeHash& rhs) {
            m_me  ^= rhs.m_me;
            m_you ^= rhs.m_you;
         }
         bool operator==(const MikeHash& rhs) const {
            return (m_me == rhs.m_me && m_you == rhs.m_you);
         }
         bool operator<(const MikeHash& rhs) const {
            return (m_me < rhs.m_me || (m_me == rhs.m_me && m_you < rhs.m_you));
         }
         bool operator!=(const MikeHash& rhs) const {
            return !(*this == rhs);
         }
         friend std::ostream& operator<< (std::ostream& os, const MikeHash& arg);
         friend class MikeBoard;
         friend class MikeGodBotV2;

      private:
         uint64_t m_me;
         uint64_t m_you;
   }; // MikeHash


   class MikeMove {
      public:
         MikeMove() {}
         MikeMove(const game::Move& move);
         friend std::ostream& operator<< (std::ostream& os, const MikeMove& arg);

         int col_from;
         int row_from;
         int col_to;
         int row_to;
         int captured;

   }; // MikeMove


   class MikeBoard {
      public:
         MikeBoard(const game::Board &board);
         void makeMove(const MikeMove& move);
         void undoMove();
         int isFinished() const;
         int getLegalMoves(MikeMove *pMove) const;
         MikeHash getHash() const {return m_hash;}
         MikeHash getHash(const MikeMove& move) const;
         int getTurn() const {return m_turn;}
         friend std::ostream& operator<< (std::ostream& os, const MikeBoard& arg);

      private:
         int m_turn;
         int m_board[8][8];
         std::vector<MikeMove> m_moveList;
         MikeHash m_hash;
   }; // MikeBoard

} // namespace players

#endif //TOUCHDOWN_MIKE_H

