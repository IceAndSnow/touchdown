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

static game::Player* selectBot(
        const std::vector<game::Player*> players,
        const std::string initialRequest) {
    int input;
    std::cout << std::endl;
    std::cout << initialRequest << std::endl;
    for(unsigned int i = 0; i < players.size(); ++i) {
        std::cout << "\t" << (i+1) << ". " << players[i]->name() << std::endl;
    }
    std::cout << "If none of the above options are chosen then the program will exit" << std::endl;
    std::cin >> input;
    if(0 < input || input <= players.size()) {
        return players[input-1];
    }
    return nullptr;
}

static inline void watchBotsVersus(std::vector<game::Player*> players) {
    game::Player* p1 = selectBot(players, "Choose a bot to play: ");
    if(p1 != nullptr) {
        game::Player* p2 = selectBot(players, "Choose another bot to play: ");
        if(p2 != nullptr) {
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

static inline bool doesVectorContain(const std::vector<game::Player*> ve, game::Player* va) {
    for(unsigned int i = 0; i < ve.size(); ++i) {
        if(ve[i] == va) {
            return true;
        }
    }
    return false;
}

static inline std::vector<game::Player*> getSelectedPlayers(std::vector<game::Player*> players) {
    std::vector<game::Player*> selectedPlayers;
    while(true) {
        std::cout << "So far the following bots are selected:" << std::endl;
        if(selectedPlayers.size() != 0) {
            std::cout << selectedPlayers[0]->name();
        }
        for(int i = 1; i < selectedPlayers.size(); ++i) {
            if(i == selectedPlayers.size() - 1) {
                std::cout << " and ";
            } else {
                std::cout << ", ";
            }
            std::cout << selectedPlayers[i]->name();
        }
        std::cout << std::endl;
        std::cout << "Please select one of the following bots by the corresponding number or type 0 to not select more bots:" << std::endl;
        for(unsigned int i = 0; i < players.size(); ++i) {
            if(!doesVectorContain(selectedPlayers, players[i])) {
                std::cout << "\t" << (i+1) << ". " << players[i]->name() << std::endl;
            }
        }
        int input;
        std::cin >> input;
        if(input == 0) {
            break;
        } else if(input < 0 || input > players.size()) {
            std::cout << "The given input (" << input << ") is not valid" << std::endl;
        } else if(doesVectorContain(selectedPlayers, players[input-1])) {
            std::cout << "That bot is already selected" << std::endl;
        } else {
            selectedPlayers.push_back(players[input-1]);
        }
    }
    return selectedPlayers;
}

static inline game::Board getBoardFromInput() {

    std::cout << "Now enter a board configuration. The standard initial board would be entered like below:" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << "0 2 0 0 0 0 1 0" << std::endl;
    std::cout << std::endl;

    game::Board result;

    int input;
    for(unsigned char x = 0; x < 8; ++x) {
        for(unsigned char y = 0; y < 8; ++y) {
            std::cin >> input;
            if(input < 0 || input > 2) {
                std::cout << "Invalid input " << input << ". Please re-enter starting from this tile (" << x << ", " << y << ")." << std::endl;
                --y;
                continue;
            } else {
                result[x][y] = input;
            }
        }
    }
    return result;
}

static inline void seeBotMove(std::vector<game::Player*> players) {
    game::Player* bot = selectBot(players, "Choose the bot: ");
    if(bot != nullptr) {
        game::Board board = getBoardFromInput();

        game::Game touchdown(bot, bot);
        touchdown.loadBoard(board);

        unsigned int activePlayer = 0;
        while(activePlayer != 1 && activePlayer != 2) {
            std::cout << "Now choose what player (1 or 2) the bot is: ";
            std::cin >> activePlayer;
        }

        touchdown.setActivePlayer(activePlayer);
        std::cout << "After the bots move the board now looks like this: " << std::endl;
        game::GameState gameState = touchdown.performNextMove();
        gameState.m_board.print();
        explicitlyPrintBoard(gameState.m_board);
        if(gameState.isGameOver()) {
            std::cout << "The bot's move ended the game. Assuming that the game was not already over before this move." << std::endl;
            if(gameState.m_status == INVALID_MOVE_PERFORMED) {
                std::cout << "Player " << (gameState.m_turn ? 1 : 2);
                std::cout << " tried performing an illegal move!" << std::endl;
                EXP_PRINT(";" << (gameState.m_turn ? 1 : 2) << ";INVALID_MOVE");
            } else if(gameState.m_status == TIE){
                std::cout << "The game ended in a tie" << std::endl;
                EXP_PRINT(";0;TIE");
            } else {
                std::cout << "The winner is: Player " << (int)gameState.m_status << " (" << gameState.m_winner->name() << ")" << std::endl;
                EXP_PRINT(";" << (int)gameState.m_status << ";WINNER");
            }
        } else {
            std::cout << "The bot's move did not end the game." << std::endl;
            EXP_PRINT(";0;NOT_YET_DECIDED");
        }

        std::cout << "Notice that the outcome may vary as some bots are not deterministic." << std::endl;
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
    std::cout << "\t2. Gather statistics from all the bots (by playing them against eachother)" << std::endl;
    std::cout << "\t3. Show available bots" << std::endl;
    std::cout << "\t4. Gather statistics from some of the bots (by playing them against eachother)" << std::endl;
    std::cout << "\t5. See what move a bot will perform on a specific board configuration" << std::endl;
    std::cout << "If none of the above options are chosen then the program will exit" << std::endl;

    int input;
    std::cin >> input;

    if(input == 1) {
        watchBotsVersus(players);
    } else if(input == 2) {
        gatherStatistics(players);
    } else if(input == 3) {
        showAvailableBots(players);
    } else if(input == 4) {
        std::vector<game::Player*> selectedPlayers = getSelectedPlayers(players);
        gatherStatistics(selectedPlayers);
    } else if(input == 5) {
        seeBotMove(players);
    }

    return 0;
}
