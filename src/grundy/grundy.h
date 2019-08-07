#ifndef TOUCHDOWN_GRUNDY_H
#define TOUCHDOWN_GRUNDY_H

#include <unordered_set>
#include <unordered_map>

namespace grundy {

    struct GrundyBoardState {

        char m_board[8][8];

        bool m_turn;

        bool operator==(const GrundyBoardState& b) const;

    };

    struct GrundyBoardStateHasher {

        std::size_t operator()(const GrundyBoardState& b) const;

    };

    class Grundy {

        std::unordered_map<GrundyBoardState, unsigned int, GrundyBoardStateHasher> memo;

        unsigned int grundyNoMemo(GrundyBoardState& state);
        unsigned int grundyForCell(std::unordered_set<unsigned int>& set, GrundyBoardState& state, const unsigned char x, const unsigned char y);

    public:

        unsigned int mex(const std::unordered_set<unsigned int> set);

        unsigned int grundy(GrundyBoardState& state);

    };

}

#endif //TOUCHDOWN_GRUNDY_H
