#include <iostream>

#include "options.h"
#include "game/game.h"
#include "players/generic_instantiator.h"
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

static inline void showGame(players::PlayerInstantiator* p1, players::PlayerInstantiator* p2) {
    std::cout << std::endl;

    game::Player* bot1 = p1->createNewPlayer();
    game::Player* bot2 = nullptr;
    if(p1 == p2) {
        bot2 = bot1;
    } else {
        bot2 = p2->createNewPlayer();
    }
    game::Game touchdown(bot1, bot2);

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

    p1->cleanUp();
    p2->cleanUp();

    gameState.m_board.print();
}

static players::PlayerInstantiator* selectBot(
        const std::vector<players::PlayerInstantiator*> playerGens,
        const std::string initialRequest) {
    int input;
    std::cout << std::endl;
    std::cout << initialRequest << std::endl;
    for(unsigned int i = 0; i < playerGens.size(); ++i) {
        std::cout << "\t" << (i+1) << ". " << playerGens[i]->name() << std::endl;
    }
    std::cout << "If none of the above options are chosen then the program will exit" << std::endl;
    std::cin >> input;
    if(0 < input || input <= playerGens.size()) {
        return playerGens[input-1];
    }
    return nullptr;
}

static inline void watchBotsVersus(std::vector<players::PlayerInstantiator*> playerGens) {
    players::PlayerInstantiator* p1 = selectBot(playerGens, "Choose a bot to play: ");
    if(p1 != nullptr) {
        players::PlayerInstantiator* p2 = selectBot(playerGens, "Choose another bot to play: ");
        if(p2 != nullptr) {
            showGame(p1, p2);
        }
    }
}

static inline void gatherStatistics(std::vector<players::PlayerInstantiator*> playerGens) {
    int input;

    std::cout << std::endl;
    std::cout << "The players/bots in this program are: " << std::endl;
    for(unsigned int i = 0; i < playerGens.size(); ++i) {
        std::cout << "\t" << playerGens[i]->name() << std::endl;
    }
    std::cout << "Choose how many times each bot plays against every other bot: ";
    std::cin >> input;
    std::cout << std::endl;

    score::HighScore highScore;

    score::Statistics stats = highScore.recordTournament(playerGens, (unsigned int)input);
    stats.print();
    stats.eprint();
}

static inline void showAvailableBots(std::vector<players::PlayerInstantiator*> playerGens) {

    std::cout << std::endl;
    std::cout << "Available bots:" << std::endl;
    for(unsigned int i = 0; i < playerGens.size(); ++i) {
        std::cout << "\t" << (i+1) << ". " << playerGens[i]->name() << std::endl;
        if(i != playerGens.size()-1) {
            EXP_PRINT(playerGens[i]->name() << std::endl);
        } else {
            EXP_PRINT(playerGens[i]->name());
        }
    }
}

static inline bool doesVectorContain(const std::vector<players::PlayerInstantiator*> ve, players::PlayerInstantiator* va) {
    for(unsigned int i = 0; i < ve.size(); ++i) {
        if(ve[i] == va) {
            return true;
        }
    }
    return false;
}

static inline std::vector<players::PlayerInstantiator*> getSelectedPlayerGens(std::vector<players::PlayerInstantiator*> playerGens) {
    std::vector<players::PlayerInstantiator*> selectedPlayerGens;
    while(true) {
        std::cout << "So far the following bots are selected:" << std::endl;
        if(selectedPlayerGens.size() != 0) {
            std::cout << selectedPlayerGens[0]->name();
        }
        for(int i = 1; i < selectedPlayerGens.size(); ++i) {
            if(i == selectedPlayerGens.size() - 1) {
                std::cout << " and ";
            } else {
                std::cout << ", ";
            }
            std::cout << selectedPlayerGens[i]->name();
        }
        std::cout << std::endl;
        std::cout << "Please select one of the following bots by the corresponding number or type 0 to not select more bots:" << std::endl;
        for(unsigned int i = 0; i < playerGens.size(); ++i) {
            if(!doesVectorContain(selectedPlayerGens, playerGens[i])) {
                std::cout << "\t" << (i+1) << ". " << playerGens[i]->name() << std::endl;
            }
        }
        int input;
        std::cin >> input;
        if(input == 0) {
            break;
        } else if(input < 0 || input > playerGens.size()) {
            std::cout << "The given input (" << input << ") is not valid" << std::endl;
        } else if(doesVectorContain(selectedPlayerGens, playerGens[input-1])) {
            std::cout << "That bot is already selected" << std::endl;
        } else {
            selectedPlayerGens.push_back(playerGens[input-1]);
        }
    }
    return selectedPlayerGens;
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

static inline void seeBotMove(std::vector<players::PlayerInstantiator*> playerGens) {
    players::PlayerInstantiator* botGen = selectBot(playerGens, "Choose the bot: ");
    if(botGen != nullptr) {
        game::Board board = getBoardFromInput();

        game::Player* bot = botGen->createNewPlayer();

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
        botGen->cleanUp();
    }
}

int main(int argc, char **argv) {
    explicitMode = false;
    for(unsigned int i = 0; i < argc; ++i) {
        if(strcmp("-e", argv[i]) == 0) { //Explicit mode
            explicitMode = true;
        }
    }

    auto randomBotGen = players::GenericInstantiator<players::RandomBot>();
    auto iceBotGen = players::GenericInstantiator<players::IceBot>();
    auto mikeBotGen = players::GenericInstantiator<players::MikeBot>();

    std::vector<players::PlayerInstantiator*> playerGens;
    playerGens.push_back(&randomBotGen);
    playerGens.push_back(&iceBotGen);
    playerGens.push_back(&mikeBotGen);

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
        watchBotsVersus(playerGens);
    } else if(input == 2) {
        gatherStatistics(playerGens);
    } else if(input == 3) {
        showAvailableBots(playerGens);
    } else if(input == 4) {
        std::vector<players::PlayerInstantiator*> selectedPlayerGens = getSelectedPlayerGens(playerGens);
        gatherStatistics(selectedPlayerGens);
    } else if(input == 5) {
        seeBotMove(playerGens);
    }

    return 0;
}
