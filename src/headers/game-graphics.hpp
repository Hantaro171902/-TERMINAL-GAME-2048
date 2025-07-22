#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <string>
#include <tuple>

using namespace std;

enum GameBoardDimensions {
  MIN_GAME_BOARD_PLAY_SIZE = 3,
  MAX_GAME_BOARD_PLAY_SIZE = 10
};
enum { COMPETITION_GAME_BOARD_PLAY_SIZE = 4 };

namespace Game {
namespace Graphics {
string AsciiArt2048();
string BoardInputPrompt();
string YouWinPrompt();
string GameOverPrompt();
string EndOfEndlessPrompt();
string InvalidInputGameBoardErrorPrompt();
string QuestionEndOfWinningGamePrompt();
string GameStateNowSavedPrompt();
string GameBoardNoSaveErrorPrompt();
string BoardSizeErrorPrompt();
string InputCommandListPrompt();
string EndlessModeCommandListPrompt();
string InputCommandListFooterPrompt();
using scoreboard_display_data_t =
  tuple<bool, string, string, string>;
  string GameScoreBoardBox(scoreboard_display_data_t scdd);
  string GameScoreBoardOverlay(scoreboard_display_data_t scdd);
  using end_screen_display_data_t = tuple<bool, bool>;
  string GameEndScreenOverlay(end_screen_display_data_t esdd);
  using input_controls_display_data_t = tuple<bool, bool>;
  string GameInputControlsOverlay(input_controls_display_data_t gamestatus);
  } // namespace Graphics
} // namespace Game

#endif
