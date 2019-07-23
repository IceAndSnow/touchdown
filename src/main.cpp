#include <iostream>
#include "grundy/grundy.h"

int main() {
    grundy::GrundyBoardState state =
            {
                {   // Player 1 moves left,
                    // Player 2 moves right
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 0, 2, 0, 0, 0, 0, 1, 0},
                    { 0, 0, 0, 0, 0, 0, 1, 0},
                    { 0, 2, 0, 0, 0, 0, 1, 0},
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 0, 0, 0, 0, 0, 0, 0, 0}
                },
                true //true: Player 1's turn, false: Player 2's turn
            };

    std::cout << "The conclusion is based on the assumption, that both players play optimally" << std::endl;

    if(grundy::grundy(state)) {
        std::cout << "The current player is winning" << std::endl;
    } else {
        std::cout << "The game will either end in a tie or the current player loses" << std::endl;
    }

    return 0;
}