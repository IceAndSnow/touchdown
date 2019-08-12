#ifndef TOUCHDOWN_MIKE_BOT_H
#define TOUCHDOWN_MIKE_BOT_H

#include "../game/player.h"
#include "mike.h"
#include <vector>
#include <map>
#include <iostream>

// This bot works by maintaining a statistics of the outcome from each position.
// This statistics consists of the number of won, drawn, and lost games from the position.
// The statistics is populated by playing a nunber of random games, where each move is
// selected from a probability distribution based on the statistics.
// This random move selection is performed in the function MikeBot::getRandomMove().
// The population of the statiscs is done by the function MikeBot::playGameToEnd().

namespace players {

   class MikeStats {
      public:
         MikeStats() : m_numWin(0), m_numDraw(0), m_numLoss(0) {}

         float getResult() const {
            float total = getTotal();
            if (total) {
               float res = (m_numWin - m_numLoss) / total;
               return res;
            }
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
         friend std::ostream& operator<< (std::ostream& os, const MikeStats& arg);
         friend MikeStats operator-(const MikeStats& rhs); // Negate

      public:
         int m_numWin;
         int m_numDraw;
         int m_numLoss;
   }; // MikeStats


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

         void trim(int num) {
            auto it = m_map.begin();
            while (it != m_map.end()) {
               auto tmp = it;
               ++it;
               if (tmp->second.getTotal() <= num) {
                  m_map.erase(tmp);
               }
            }
         } // trim

         friend std::ostream& operator<< (std::ostream& os, const MikeResults& arg);

      private:
         std::map<MikeHash, MikeStats> m_map;
   }; // MikeResults


   class MikeBot : public game::Player {

      public:

         MikeBot();

         std::string name() const;

         bool preferToStart(const game::Board& board);

         game::Move play(const game::Board& board);

      private:
         const MikeMove& getRandomMove(const MikeBoard& board, const MikeMove *moveList, int moveCount) const;
         int             playGameToEnd(const MikeBoard &startBoard, int num);

         MikeResults m_results;
   }; // class MikeBot

} // namespace players

#endif //TOUCHDOWN_MIKE_BOT_H

