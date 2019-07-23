#include <iostream>

#include "game/game.h"
#include "players/random_bot.h"

int main() {

    players::RandomBot player1, player2;

    game::Game touchdown(&player1, &player2);

    while(true) {
        game::GameState gameState = touchdown.performNextMove();

        if(gameState.isGameOver()) {
            break;
        }

    }

    unsigned char winner = touchdown.performNextMove().m_winner;

    std::cout << "The winner is: ";

    if(winner == PLAYER_1) {
        std::cout << "Player 1" << std::endl;
    }
    if(winner == PLAYER_2) {
        std::cout << "Player 2" << std::endl;
    }
    if(winner == INVALID_MOVE_PERFORMED) {
        std::cout << "Something went wrong" << std::endl;
    }

    std::cout << "The final board position is: " << std::endl;

    touchdown.performNextMove().m_board.print();

    return 0;
}