#include <iostream>

#include "options.h"
#include "game/game.h"
#include "players/ice_bot.h"
#include "players/random_bot.h"
#include "players/mike_bot.h"
#include "score/high_score.h"
#include <string.h>

bool explicitMode = false;

static inline void explicitlyPrintBoard(const game::Board& b) {
    for(unsigned int y = 0; y < 8; ++y) {
        for(unsigned int x = 0; x < 8; ++x) {
            EXP_PRINT((int)b[x][y]);
            if(y != 7 || x != 7) {
                EXP_PRINT(":");
            }
        }
    }
}

static inline void showGame(game::Player* p1, game::Player* p2) {
    std::cout << std::endl;

    game::Game touchdown(p1, p2);

    game::GameState initialGameState = touchdown.getCurrentState();

    std::cout << "Player 1 is " << initialGameState.m_player1->name() << " and Player 2 is " << initialGameState.m_player2->name() << std::endl;
    EXP_PRINT(initialGameState.m_player1->name() << ";" << initialGameState.m_player2->name() << ";");
    std::cout << "The board initially looks like this: " << std::endl;
    explicitlyPrintBoard(initialGameState.m_board);

    bool gameOver = false;

    while(!gameOver) {
        game::GameState s = touchdown.performNextMove();
        gameOver = s.isGameOver();

        EXP_PRINT(";");
        explicitlyPrintBoard(s.m_board);

        if(!gameOver) {
            s.m_board.print();

            std::cout << "Now it is Player " << (s.m_turn ? 1 : 2) << "'s (" << (s.m_turn ? s.m_player1->name() : s.m_player2->name()) << ") turn" << std::endl;
        }
    }

    game::GameState gameState = touchdown.performNextMove();

    if(gameState.m_status == INVALID_MOVE_PERFORMED) {
        std::cout << "Player " << (gameState.m_turn ? 1 : 2);
        std::cout << " tried performing an illegal move!" << std::endl;
        EXP_PRINT(";" << (gameState.m_turn ? 1 : 2) << ";INVALID_MOVE");
    } else if(gameState.m_status == TIE){
        std::cout << "The game ended in a tie between Player 1 (" << gameState.m_player1->name() << ") and Player 2 (" << gameState.m_player2->name() << ")" << std::endl;
        EXP_PRINT(";0;TIE");
    } else {
        std::cout << "The winner is: Player " << (int)gameState.m_status << " (" << gameState.m_winner->name() << ")" << std::endl;
        EXP_PRINT(";" << (int)gameState.m_status << ";WINNER");
    }

    std::cout << "The final board position is: " << std::endl;

    gameState.m_board.print();
}

static inline void watchBotsVersus(std::vector<game::Player*> players) {
    int input;

    std::cout << std::endl;
    std::cout << "Choose a bot to play: " << std::endl;
    for(unsigned int i = 0; i < players.size(); ++i) {
        std::cout << "\t" << (i+1) << ". " << players[i]->name() << std::endl;
    }
    std::cout << "If none of the above options are chosen then the program will exit" << std::endl;
    std::cin >> input;
    if(0 < input || input <= players.size()) {
        game::Player* p1 = players[input-1];

        std::cout << std::endl;
        std::cout << "Choose another bot to play: " << std::endl;
        for(unsigned int i = 0; i < players.size(); ++i) {
            std::cout << "\t" << (i+1) << ". " << players[i]->name() << std::endl;
        }
        std::cout << "If none of the above options are chosen then the program will exit" << std::endl;
        std::cin >> input;
        if(0 < input || input <= players.size()) {
            game::Player* p2 = players[input-1];

            showGame(p1, p2);
        }
    }
}

static inline void gatherStatistics(std::vector<game::Player*> players) {
    int input;

    std::cout << std::endl;
    std::cout << "The players/bots in this program are: " << std::endl;
    for(unsigned int i = 0; i < players.size(); ++i) {
        std::cout << "\t" << players[i]->name() << std::endl;
    }
    std::cout << "Choose how many times each bot plays against every other bot: ";
    std::cin >> input;
    std::cout << std::endl;

    score::HighScore highScore;

    score::Statistics stats = highScore.recordTournament(players, (unsigned int)input);
    stats.print();
    stats.eprint();
}

static inline void showAvailableBots(std::vector<game::Player*> players) {

    std::cout << std::endl;
    std::cout << "Available bots:" << std::endl;
    for(unsigned int i = 0; i < players.size(); ++i) {
        std::cout << "\t" << (i+1) << ". " << players[i]->name() << std::endl;
        if(i != players.size()-1) {
            EXP_PRINT(players[i]->name() << std::endl);
        } else {
            EXP_PRINT(players[i]->name());
        }
    }
}

int main(int argc, char **argv) {

    explicitMode = false;
    for(unsigned int i = 0; i < argc; ++i) {
        if(strcmp("-e", argv[i]) == 0) { //Explicit mode
            explicitMode = true;
        }
    }

    players::RandomBot randomBot;
    players::IceBot iceBot;
    players::MikeBot mikeBot;

    std::vector<game::Player*> players;
    players.push_back(&randomBot);
    players.push_back(&iceBot);
    players.push_back(&mikeBot);

    std::cout << "Choose one of the following options:" << std::endl;
    std::cout << "\t1. Watch 2 bots play against each other" << std::endl;
    std::cout << "\t2. Gather statistics from all the bots (by playing them against eachother" << std::endl;
    std::cout << "\t3. Show available bots" << std::endl;
    std::cout << "If none of the above options are chosen then the program will exit" << std::endl;

    int input;
    std::cin >> input;

    if(input == 1) {
        watchBotsVersus(players);
    } else if(input == 2) {
        gatherStatistics(players);
    } else if(input == 3) {
        showAvailableBots(players);
    }

    return 0;
}
