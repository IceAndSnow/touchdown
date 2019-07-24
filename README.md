Touchdown
=========

This repository explores the game of Touchdown.
Thanks to [MJoergen](https://github.com/MJoergen) for providing the rules.

## How to run

If `make` is installed, you can simply type `make run` in the terminal to run the program.
To build the program simply type `make Touchdown`.

## Rules of the game

The initial configuration of the game is a chess-board in it's initial configuration but with only the pawns on the board.
Each pawn can move 1 tile at a time towards the side opposite it's starting side if there is no piece in that tile.
A pawn can also move 1 tile diagonally forward if a pawn of another color is on the tile (this will remove the pawn originally on the tile from the game).
Player 1 controls the white pawns, and player 2 controls the black pawns.
Player 1 starts by making a move (i.e. moving a single pawn a single tile).
Then player 2 makes a move, then player 1, then player 2 and so on.
This is the case until:
 - A player gets a pawn to a tile behind the other players starting line. In this case, the player whose pawn crossed the other players starting line wins.
 - A player has no more valid moves on the player's turn. This results in a tie.

## Implementing a bot to play in this program

Simply create a class that implements the `game::Player` interface.

The `std::string name() const` method should return the name of the bot.

The `bool preferToStart(const game::Board& board)` method should return a boolean indicating if the bot wants to start given that the configuration is as the given board.
The tile-value of 1 on the given board will indicate that the starting player has a pawn there at the beginning of the game.
Likewise a tile-value of 2 on the given board will indicate that the second player has a pawn there at the beginning of the game.
A board tile of value 0 indicates that no pawn is on that tile.
Player 1's winning condition is to get a pawn on one of the tiles: `board[?][0]`.
And Player 2's winning condition is to get a pawn on one of the tiles: `board[?][7]`.


The `game::Move play(const game::Board& board)` method should return a valid move given the board is configured as the given board.
It can be assumed that at least 1 valid move always will be present.
A board tile of value 1 indicates that the bot has a pawn at that spot.
A board tile of value 2 indicates that the opposing player has a pawn at that spot.
A board tile of value 0 indicates that no pawn is at that tile.
The bot's winning condition is to get a pawn on one of the tiles: `board[?][0]`.
The opposing player's winning condition is to get a pawn on one of the tiles: `board[?][7]`.
