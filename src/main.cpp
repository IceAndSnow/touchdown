#include <iostream>

#include "game/game.h"
#include "players/random_bot.h"

int main() {

    players::RandomBot player1, player2;

    game::Game touchdown(&player1, &player2);

    while(!touchdown.performNextMove().isGameOver()) {  }

    game::GameState gameState = touchdown.performNextMove();

    if(gameState.m_status == INVALID_MOVE_PERFORMED) {
        std::cout << "Player " << (gameState.m_turn ? 1 : 2);
        std::cout << " tried performing an illegal move!" << std::endl;
    } else {
        std::cout << "The winner is: Player " << (int)gameState.m_status << " (" << gameState.m_winner->name() << ")" << std::endl;
    }

    std::cout << "The final board position is: " << std::endl;

    gameState.m_board.print();

    return 0;
}