#ifndef TOUCHDOWN_GRUNDY_H
#define TOUCHDOWN_GRUNDY_H

#include <unordered_set>

namespace grundy {

    struct GrundyBoardState {

        char m_board[8][8];

        bool m_turn;

        bool operator==(const GrundyBoardState& b) const;

    };

    struct GrundyBoardStateHasher {

        std::size_t operator()(const GrundyBoardState& b) const;

    };

    unsigned int mex(const std::unordered_set<unsigned int> set);

    unsigned int grundy(GrundyBoardState& state);

}

#endif //TOUCHDOWN_GRUNDY_H
