#ifndef TOUCHDOWN_GENERIC_INSTANTIATOR_H
#define TOUCHDOWN_GENERIC_INSTANTIATOR_H

#include "player_instantiator.h"

namespace players {

    template<typename TPlayer>
    class GenericInstantiator : public PlayerInstantiator {

    private:
        TPlayer* m_player;

    public:
        GenericInstantiator() {
            m_player = nullptr;
        }

        virtual game::Player* createNewPlayer() {
            if(m_player != nullptr) {
                cleanUp();
            }
            m_player = new TPlayer();
            return m_player;
        };

        virtual std::string name() {
            TPlayer p;
            return p.name();
        }

        virtual void cleanUp() {
            delete m_player;
            m_player = nullptr;
        };

    };

}

#endif //TOUCHDOWN_GENERIC_INSTANTIATOR_H
