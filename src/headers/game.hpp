#ifndef GAME_H
#define GAME_H

#include <string>

using namespace std;

namespace Game {
struct GameBoard;

enum class PlayGameFlag { BrandNewGame, ContinuePreviousGame };
void playGame(PlayGameFlag cont, GameBoard gb,
              unsigned long long userInput_PlaySize = 1);
void startGame();
void continueGame(const string& filename);
void removeTiles(GameBoard& gb);

}; // namespace Game

#endif
