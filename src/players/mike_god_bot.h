#ifndef TOUCHDOWN_MIKE_GOD_BOT_H
#define TOUCHDOWN_MIKE_GOD_BOT_H

#include "../game/player.h"
#include "mike.h"
#include <vector>
#include <map>
#include <iostream>

// This bot works by the good-ol-fashioned minimax search with a fairly simple
// heuristic evaluation function.

namespace players {

   class MikeGodBot : public game::Player {

      public:

         MikeGodBot();

         std::string name() const;

         bool preferToStart(const game::Board& board);

         game::Move play(const game::Board& board);

      private:
         int eval(const MikeBoard& board);
         int search(MikeBoard board, float alpha, float beta, int level);
   }; // class MikeGodBot

} // namespace players

#endif //TOUCHDOWN_MIKE_GOD_BOT_H

