#ifndef TOUCHDOWN_PLAYER_INSTANTIATOR_H
#define TOUCHDOWN_PLAYER_INSTANTIATOR_H

#include "../game/player.h"
#include <string>

namespace players {

    class PlayerInstantiator {

    public:

        virtual game::Player* createNewPlayer() = 0;
        virtual std::string name() = 0;
        virtual void cleanUp() = 0;
    };

}

#endif //TOUCHDOWN_PLAYER_INSTANTIATOR_H