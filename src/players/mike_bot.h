#ifndef TOUCHDOWN_MIKE_BOT_H
#define TOUCHDOWN_MIKE_BOT_H

#include "../game/player.h"
#include <vector>
#include <map>

namespace players {

   static inline uint32_t mikeRand() {
      uint32_t v1 = rand();
      uint32_t v2 = rand() << 12;
      uint32_t v3 = rand() << 24;
      return v3 + (v2 ^ v1);
   } // end of mikeRand

   class MikeHash {
      public:
         MikeHash() : m_hi(mikeRand()), m_lo(mikeRand()) {}
         void update(const MikeHash& rhs) {
            m_hi ^= rhs.m_hi;
            m_lo ^= rhs.m_lo;
         }
         bool operator==(const MikeHash& rhs) const {
            return (m_hi == rhs.m_hi && m_lo == rhs.m_lo);
         }
         bool operator<(const MikeHash& rhs) const {
            return (m_hi < rhs.m_hi || (m_hi == rhs.m_hi && m_lo < rhs.m_lo));
         }
         bool operator!=(const MikeHash& rhs) const {
            return !(*this == rhs);
         }
      private:
      public: // Debug
         uint32_t m_hi;
         uint32_t m_lo;
   }; // MikeHash

   class MikeMove {
      public:
         MikeMove() {}
         MikeMove(const game::Move& move);
         std::string toStr() const;

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
         void print() const;

      private:
         int m_turn;
         int m_board[8][8];
         std::vector<MikeMove> m_moveList;
         MikeHash m_hash;

         // These are set in constructor.
         const MikeHash m_white[8][8];
         const MikeHash m_black[8][8];
         const MikeHash m_whiteToMove;
   }; // MikeBoard

   class MikeStats {
      public:
         MikeStats() : m_numWin(0), m_numDraw(0), m_numLoss(0) {}

         float getResult() const {
            float total = getTotal();
            if (total)
               return (m_numWin - m_numLoss) / total;
            return 0.0;
         } // getResult

         int getTotal() const {
            return m_numWin + m_numDraw + m_numLoss;
         } // getTotal

         void update(int result) {
            switch (result) {
               case  1 : ++m_numWin;  break;
               case  0 : ++m_numDraw; break;
               case -1 : ++m_numLoss; break;
            }
         } // update

      public:
         int m_numWin;
         int m_numDraw;
         int m_numLoss;
   }; // MikeStats

   MikeStats operator-(const MikeStats& rhs);

   class MikeResults {
      public:
         void update(const MikeHash& hash, int res) {
            m_map[hash].update(res);
         } // update

         MikeStats operator[] (const MikeHash& hash) const {
            MikeStats res;
            auto it = m_map.find(hash);
            if (it != m_map.end())
               res = it->second;
            return res;
         } // operator []

         void trim() {
            auto it = m_map.begin();
            while (it != m_map.end()) {
               auto tmp = it;
               ++it;
               if (tmp->second.getTotal() <= 1) {
                  m_map.erase(tmp);
               }
            }
         } // trim

         void print() const {
            printf("Results contains %ld elements:\n", m_map.size());
            for (auto it = m_map.begin(); it != m_map.end(); ++it) {
               printf("%08x.%08x -> %f\n", it->first.m_hi, it->first.m_lo, it->second.getResult());
            }
         } // print

      private:
         std::map<MikeHash, MikeStats> m_map;
   }; // MikeResults

   class MikeBot : public game::Player {

      public:

         MikeBot();
         MikeBot(unsigned int seed);

         std::string name() const;

         bool preferToStart(const game::Board& board);

         game::Move play(const game::Board& board);

      private:
         const MikeMove&   getRandomMove(const MikeBoard& board, const MikeMove *moveList, int moveCount) const;
         int               playGameToEnd(const MikeBoard &startBoard);
         float             search(       const MikeBoard &board);


         MikeResults m_results;
   }; // class MikeBot

} // namespace players

#endif //TOUCHDOWN_MIKE_BOT_H

