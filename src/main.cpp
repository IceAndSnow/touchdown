#include <iostream>

#include "game/game.h"
#include "players/ice_bot.h"
#include "players/random_bot.h"

int main() {

    players::RandomBot player1;
    players::IceBot player2;

    game::Game touchdown(&player1, &player2);

    bool gameOver = false;

    while(!gameOver) {
        game::GameState s = touchdown.performNextMove();
        gameOver = s.isGameOver();

        if(!gameOver) {
            s.m_board.print();

            std::cout << "Now it is Player " << (s.m_turn ? 1 : 2) << "'s (" << (s.m_turn ? s.m_player1->name() : s.m_player2->name()) << ") turn" << std::endl;
        }
    }

    game::GameState gameState = touchdown.performNextMove();

    if(gameState.m_status == INVALID_MOVE_PERFORMED) {
        std::cout << "Player " << (gameState.m_turn ? 1 : 2);
        std::cout << " tried performing an illegal move!" << std::endl;
    } else if(gameState.m_status == TIE){
        std::cout << "The game ended in a tie between Player 1 (" << gameState.m_player1->name() << ") and Player 2 (" << gameState.m_player2->name() << ")" << std::endl;
    } else {
        std::cout << "The winner is: Player " << (int)gameState.m_status << " (" << gameState.m_winner->name() << ")" << std::endl;
    }

    std::cout << "The final board position is: " << std::endl;

    gameState.m_board.print();

    return 0;
}