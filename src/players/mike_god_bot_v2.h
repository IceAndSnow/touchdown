#ifndef TOUCHDOWN_MIKE_GOD_BOT_V2_H
#define TOUCHDOWN_MIKE_GOD_BOT_V2_H

#include "../game/player.h"
#include "mike.h"
#include <vector>
#include <map>
#include <iostream>

// This bot works by the good-ol-fashioned minimax search with a fairly simple
// heuristic evaluation function.

namespace players {

   class MikeGodBotV2 : public game::Player {

      public:

         MikeGodBotV2();
         MikeGodBotV2(unsigned int seed);

         std::string name() const;

         bool preferToStart(const game::Board& board);

         game::Move play(const game::Board& board);

      private:
         int eval(const MikeBoard& board);
         int search(MikeBoard board, float alpha, float beta, int level);
   }; // class MikeGodBotV2

} // namespace players

#endif //TOUCHDOWN_MIKE_GOD_BOT_V2_H

