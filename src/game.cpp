#include "game.hpp"
#include "game-graphics.hpp"
#include "game-input.hpp"
#include "game-pregamemenu.hpp"
#include "gameboard-graphics.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include "loadresource.hpp"
#include "menu.hpp"
#include "saveresource.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <array>
#include <chrono>
#include <iostream>
#include <sstream>
#include <random> // Required for generating random numbers/integers in the RemoveTiles() function.
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>


using namespace std;
namespace Game {
namespace {
enum Directions { UP, DOWN, RIGHT, LEFT };

enum GameStatusFlag {
  FLAG_WIN,
  FLAG_END_GAME,
  FLAG_ONE_SHOT,
  FLAG_SAVED_GAME,
  FLAG_INPUT_ERROR,
  FLAG_ENDLESS_MODE,
  FLAG_GAME_IS_ASKING_QUESTION_MODE,
  FLAG_QUESTION_STAY_OR_QUIT,
  FLAG_TILES_REMOVED,           // Indicates if tiles have already been removed
  MAX_NO_GAME_STATUS_FLAGS
};

using gamestatus_t = array<bool, MAX_NO_GAME_STATUS_FLAGS>;

using gamestatus_gameboard_t = tuple<gamestatus_t, GameBoard>;

/**
 * @brief Processes the game logic for the current game state.
 * 
 * This function updates the game status and game board based on the current state.
 * It handles tile movements, checks for game-winning conditions, and verifies if the game can continue.
 * If the player cannot make any more moves, it prompts the player to remove random tiles before ending the game.
 * 
 * @param gsgb A tuple containing the current game status and game board.
 * @return gamestatus_gameboard_t A tuple containing the updated game status and game board.
 */
gamestatus_gameboard_t process_gamelogic(gamestatus_gameboard_t gsgb) {
  gamestatus_t gamestatus;
  GameBoard gb;
  tie(gamestatus, gb) = gsgb;
  unblockTilesOnGameboard(gb);
  if (gb.moved) {
    addTileOnGameboard(gb);
    registerMoveByOneOnGameboard(gb);
  }

  if (!gamestatus[FLAG_ENDLESS_MODE]) {
    if (hasWonOnGameboard(gb)) {
      gamestatus[FLAG_WIN] = true;
      gamestatus[FLAG_GAME_IS_ASKING_QUESTION_MODE] = true;
      gamestatus[FLAG_QUESTION_STAY_OR_QUIT] = true;
    }
  }
  if (!canMoveOnGameboard(gb)) {
    if(gamestatus[FLAG_TILES_REMOVED] == false)
    {
      char input;
      cout << "You lose. Do you want to remove random tiles (Y/N) ?" << endl;
      cin >> input;

      if(input == 'Y' || input == 'y')
      {
        gamestatus[FLAG_TILES_REMOVED] = true;
        removeTiles(gb);
      }
      else if (input == 'N' || input == 'n')
      {
        gamestatus[FLAG_END_GAME] = true;
        gamestatus[FLAG_START_MENU] = true;
      }
      else 
      {
        cout << "Invalid input." << endl;
      }
    }
    else
    {
      gamestatus[FLAG_END_GAME] = true;
    }
  }
  return make_tuple(gamestatus, gb);
}

using competition_mode_t = bool;
Graphics::scoreboard_display_data_t
make_scoreboard_display_data(ull bestScore, competition_mode_t cm,
                             GameBoard gb) {
  const auto gameboard_score = gb.score;
  const auto tempBestScore = (bestScore < gb.score ? gb.score : bestScore);
  const auto comp_mode = cm;
  const auto movecount = MoveCountOnGameBoard(gb);
  const auto time_str = secondsFormat(gb.duration);   // Format time as a string

  const auto scdd =
      make_tuple(comp_mode, to_string(gameboard_score),
                      to_string(tempBestScore), to_string(movecount), time_str);
  return scdd;
};

Graphics::input_controls_display_data_t
make_input_controls_display_data(gamestatus_t gamestatus) {
  const auto icdd = make_tuple(gamestatus[FLAG_ENDLESS_MODE],
                                    gamestatus[FLAG_QUESTION_STAY_OR_QUIT]);
  return icdd;
};

string DisplayGameQuestionsToPlayerPrompt(gamestatus_t gamestatus) {
  using namespace Graphics;

  ostringstream str_os;
  DrawOnlyWhen(str_os, gamestatus[FLAG_QUESTION_STAY_OR_QUIT],
               QuestionEndOfWinningGamePrompt);
  return str_os.str();
}

// NOTE: current_game_session_t
// : (bestScore, is_competition_mode, gamestatus, gamePlayBoard)
using current_game_session_t =
    tuple<ull, competition_mode_t, gamestatus_t, GameBoard>;
enum tuple_cgs_t_idx {
  IDX_BESTSCORE,
  IDX_COMP_MODE,
  IDX_GAMESTATUS,
  IDX_GAMEBOARD
};

string drawGraphics(current_game_session_t cgs) {
  // Graphical Output has a specific ordering...
  using namespace Graphics;
  using namespace Gameboard::Graphics;
  using tup_idx = tuple_cgs_t_idx;
  const auto bestScore = get<tup_idx::IDX_BESTSCORE>(cgs);
  const auto comp_mode = get<tup_idx::IDX_COMP_MODE>(cgs);
  const auto gb = get<tup_idx::IDX_GAMEBOARD>(cgs);
  const auto gamestatus = get<tup_idx::IDX_GAMESTATUS>(cgs);

  ostringstream str_os;

  // 1. Clear screen
  clearScreen();

  // 2. Draw Game Title Art
  DrawAlways(str_os, AsciiArt2048);

  // 3. Draw Scoreboard of current game session
  const auto scdd = make_scoreboard_display_data(bestScore, comp_mode, gb);
  DrawAlways(str_os, DataSuppliment(scdd, GameScoreBoardOverlay));

  // 4. Draw current 2048 game active gameboard
  DrawAlways(str_os, DataSuppliment(gb, GameBoardTextOutput));

  // 5. Draw anyinstant status feedback, like
  // "Game saved!" (which disappers after next key input).
  DrawOnlyWhen(str_os, gamestatus[FLAG_SAVED_GAME], GameStateNowSavedPrompt);

  // 6. Draw any "questions to the player" (from the game) text output
  DrawOnlyWhen(str_os, gamestatus[FLAG_GAME_IS_ASKING_QUESTION_MODE],
               DataSuppliment(gamestatus, DisplayGameQuestionsToPlayerPrompt));

  // 7. Draw Keyboard / Input Keycodes to the player
  const auto input_controls_display_data =
      make_input_controls_display_data(gamestatus);
  DrawAlways(str_os, DataSuppliment(input_controls_display_data,
                                    GameInputControlsOverlay));

  // 8. Draw any game error messages to the player (to do with keyboard input)
  DrawOnlyWhen(str_os, gamestatus[FLAG_INPUT_ERROR],
               InvalidInputGameBoardErrorPrompt);

  return str_os.str();
}

gamestatus_t update_one_shot_display_flags(gamestatus_t gamestatus) {
  const auto disable_one_shot_flag = [](bool &trigger) { trigger = !trigger; };
  if (gamestatus[FLAG_ONE_SHOT]) {
    disable_one_shot_flag(gamestatus[FLAG_ONE_SHOT]);
    // Turn off display flag: [Saved Game]
    if (gamestatus[FLAG_SAVED_GAME]) {
      disable_one_shot_flag(gamestatus[FLAG_SAVED_GAME]);
    }
    // Turn off display flag: [Input Error]
    if (gamestatus[FLAG_INPUT_ERROR]) {
      disable_one_shot_flag(gamestatus[FLAG_INPUT_ERROR]);
    }
  }
  return gamestatus;
}

using bool_gamestatus_t = tuple<bool, gamestatus_t>;
/**
 * @brief Processes non-standard input commands and updates the game status accordingly.
 * 
 * This function handles additional input commands that are not part of the standard gameplay.
 * It updates the game status based on the input character received. The function supports saving
 * the game, quitting endless mode, and returning to the main menu.
 * 
 * @param c The input character received from the user.
 * @param gamestatus The current game status flags.
 * @return bool_gamestatus_t A tuple containing a boolean indicating if the keycode is invalid and the updated game status flags.
 * 
 * @note Changes in the new version:
 * - Added support for returning to the main menu with CODE_RETURN_TO_MENU.
 * - When CODE_RETURN_TO_MENU is detected, the appropriate main menu flags are set.
 */
bool_gamestatus_t check_input_other(char c, gamestatus_t gamestatus) {
  using namespace Input::Keypress::Code;
  auto is_invalid_keycode{true};
  switch (toupper(c)) {
  case CODE_HOTKEY_ACTION_SAVE:
  case CODE_HOTKEY_ALTERNATE_ACTION_SAVE:
    gamestatus[FLAG_ONE_SHOT] = true;
    gamestatus[FLAG_SAVED_GAME] = true;
    is_invalid_keycode = false;
    break;
  case CODE_HOTKEY_QUIT_ENDLESS_MODE:
    if (gamestatus[FLAG_ENDLESS_MODE]) {
      gamestatus[FLAG_END_GAME] = true;
      is_invalid_keycode = false;
    }
    break;
  case CODE_RETURN_TO_MENU:
    // When CODE_RETURN_TO_MENU is detected, the main menu is set to start, and game flags are adjusted.    mainmenustatus[FLAG_START_MENU] = true;
    mainmenustatus[FLAG_START_GAME] = false;
    mainmenustatus[FLAG_CONTINUE_GAME] = false;
    is_invalid_keycode = false;
    break;
  }
  return make_tuple(is_invalid_keycode, gamestatus);
}
using intendedmove_gamestatus_t =
    tuple<Input::intendedmove_t, gamestatus_t>;
intendedmove_gamestatus_t
receive_agent_input(Input::intendedmove_t intendedmove,
                    gamestatus_t gamestatus) {
  using namespace Input;
  const bool game_still_in_play =
      !gamestatus[FLAG_END_GAME] && !gamestatus[FLAG_WIN];
  if (game_still_in_play) {
    // Game still in play. Take input commands for next turn.
    char c;
    getKeypressDownInput(c);
    // Update agent's intended move flags per control scheme (if flagged).
    const auto is_invalid_keypress_code = check_input_ansi(c, intendedmove) &&
                                          check_input_wasd(c, intendedmove) &&
                                          check_input_vim(c, intendedmove);
    bool is_invalid_special_keypress_code;
    tie(is_invalid_special_keypress_code, gamestatus) =
        check_input_other(c, gamestatus);
    if (is_invalid_keypress_code && is_invalid_special_keypress_code) {
      gamestatus[FLAG_ONE_SHOT] = true;
      gamestatus[FLAG_INPUT_ERROR] = true;
    }
  }
  return make_tuple(intendedmove, gamestatus);
}

GameBoard decideMove(Directions d, GameBoard gb) {
  switch (d) {
  case UP:
    tumbleTilesUpOnGameboard(gb);
    break;

  case DOWN:
    tumbleTilesDownOnGameboard(gb);
    break;

  case LEFT:
    tumbleTilesLeftOnGameboard(gb);
    break;

  case RIGHT:
    tumbleTilesRightOnGameboard(gb);
    break;
  }
  return gb;
}

using bool_gameboard_t = tuple<bool, GameBoard>;
bool_gameboard_t process_agent_input(Input::intendedmove_t intendedmove,
                                     GameBoard gb) {
  using namespace Input;
  if (intendedmove[FLAG_MOVE_LEFT]) {
    gb = decideMove(LEFT, gb);
  }
  if (intendedmove[FLAG_MOVE_RIGHT]) {
    gb = decideMove(RIGHT, gb);
  }
  if (intendedmove[FLAG_MOVE_UP]) {
    gb = decideMove(UP, gb);
  }
  if (intendedmove[FLAG_MOVE_DOWN]) {
    gb = decideMove(DOWN, gb);
  }

  return make_tuple(true, gb);
}

bool check_input_check_to_end_game(char c) {
  using namespace Input::Keypress::Code;
  switch (toupper(c)) {
  case CODE_HOTKEY_CHOICE_NO:
    return true;
  }
  return false;
}

bool continue_playing_game(istream &in_os) {
  char letter_choice;
  in_os >> letter_choice;
  if (check_input_check_to_end_game(letter_choice)) {
    return false;
  }
  return true;
}

/**
 * @brief Processes the current game status and updates the game loop control.
 * 
 * This function evaluates the current game status flags and takes appropriate actions:
 * - It handles winning conditions and prompts the user to continue playing.
 * - It checks for the end of the game conditions.
 * - It manages saving the game state by prompting the user for a filename.
 * - It resets the question asking event trigger for a new loop cycle.
 * 
 * @param gsgb A tuple containing the current game status and game board.
 * @return bool_gamestatus_t A tuple containing a boolean indicating whether to continue the game loop and the updated game status flags.
 * 
 * @note Changes in the new version:
 * - Added a prompt asking the user to enter a filename when saving the game state.
 */
bool_gamestatus_t process_gameStatus(gamestatus_gameboard_t gsgb) {
  gamestatus_t gamestatus;
  GameBoard gb;
  tie(gamestatus, gb) = gsgb;
  auto loop_again{true};
  if (!gamestatus[FLAG_ENDLESS_MODE]) {
    if (gamestatus[FLAG_WIN]) {
      if (continue_playing_game(cin)) {
        gamestatus[FLAG_ENDLESS_MODE] = true;
        gamestatus[FLAG_QUESTION_STAY_OR_QUIT] = false;
        gamestatus[FLAG_WIN] = false;
      } else {
        loop_again = false;
      }
    }
  }
  if (gamestatus[FLAG_END_GAME]) {
    // End endless_mode;
    loop_again = false;
  }
  if (gamestatus[FLAG_SAVED_GAME]) {
    cout << "Please enter the filename to save the game state" << endl;
    string filename;
    cin >> filename;
    Saver::saveGamePlayState(gb, filename);
  }

  // New loop cycle: reset question asking event trigger
  gamestatus[FLAG_GAME_IS_ASKING_QUESTION_MODE] = false;
  return make_tuple(loop_again, gamestatus);
}

using bool_current_game_session_t = tuple<bool, current_game_session_t>;
bool_current_game_session_t soloGameLoop(current_game_session_t cgs) {
  using namespace Input;
  using tup_idx = tuple_cgs_t_idx;
  const auto gamestatus =
      addressof(get<tup_idx::IDX_GAMESTATUS>(cgs));
  const auto gb = addressof(get<tup_idx::IDX_GAMEBOARD>(cgs));

  tie(*gamestatus, *gb) =
      process_gamelogic(make_tuple(*gamestatus, *gb));

  DrawAlways(cout, DataSuppliment(cgs, drawGraphics));
  *gamestatus = update_one_shot_display_flags(*gamestatus);

  intendedmove_t player_intendedmove{};
  tie(player_intendedmove, *gamestatus) =
      receive_agent_input(player_intendedmove, *gamestatus);
  tie(ignore, *gb) = process_agent_input(player_intendedmove, *gb);

  bool loop_again;
  tie(loop_again, *gamestatus) =
      process_gameStatus(make_tuple(*gamestatus, *gb));
  return make_tuple(loop_again, cgs);
}

Graphics::end_screen_display_data_t
make_end_screen_display_data(gamestatus_t world_gamestatus) {
  const auto esdd = make_tuple(world_gamestatus[FLAG_WIN],
                                    world_gamestatus[FLAG_ENDLESS_MODE]);
  return esdd;
};

string drawEndGameLoopGraphics(current_game_session_t finalgamestatus) {
  // Graphical Output has a specific ordering...
  using namespace Graphics;
  using namespace Gameboard::Graphics;
  using tup_idx = tuple_cgs_t_idx;
  const auto bestScore = get<tup_idx::IDX_BESTSCORE>(finalgamestatus);
  const auto comp_mode = get<tup_idx::IDX_COMP_MODE>(finalgamestatus);
  const auto gb = get<tup_idx::IDX_GAMEBOARD>(finalgamestatus);
  const auto end_gamestatus =
      get<tup_idx::IDX_GAMESTATUS>(finalgamestatus);

  ostringstream str_os;

  // 1. Clear screen
  clearScreen();

  // 2. Draw Game Title Art
  DrawAlways(str_os, AsciiArt2048);

  // 3. Draw Scoreboard of ending current game session
  const auto scdd = make_scoreboard_display_data(bestScore, comp_mode, gb);
  DrawAlways(str_os, DataSuppliment(scdd, GameScoreBoardOverlay));

  // 4. Draw snapshot of ending 2048 session's gameboard
  DrawAlways(str_os, DataSuppliment(gb, GameBoardTextOutput));

  // 5. Draw "You win!" or "You Lose" prompt, only if not in endless mode.
  const auto esdd = make_end_screen_display_data(end_gamestatus);
  DrawAlways(str_os, DataSuppliment(esdd, GameEndScreenOverlay));

  return str_os.str();
}

/**
 * @brief Runs the endless game loop until the game ends or the user returns to the menu.
 * 
 * This function manages the continuous gameplay loop for the endless game mode. 
 * It repeatedly calls the solo game loop until the game ends or the user chooses to return to the menu.
 * The game status and game board are updated accordingly in each iteration.
 * 
 * @param currentBestScore The current best score achieved.
 * @param cm The competition mode settings.
 * @param gb The current game board state.
 * @return GameBoard The final state of the game board after the loop ends.
 * 
 * @note Changes in the new version:
 * - Added checks to monitor if the current state is set to GAME or MENU.
 * - The loop will terminate if the user presses the M key and the state changes to MENU.
 */
GameBoard endlessGameLoop(ull currentBestScore, competition_mode_t cm,
                          GameBoard gb) {
  auto loop_again{true};
  auto currentgamestatus =
      make_tuple(currentBestScore, cm, gamestatus_t{}, gb);
  // Monitor the loop to check if the current state is still set to GAME
  // If not, then the M key was pressed, setting the current state to MENU
  // Thus, we return to the menu
  while (loop_again && ((mainmenustatus[FLAG_START_GAME] == true) || (mainmenustatus[FLAG_CONTINUE_GAME] == true))) {
    tie(loop_again, currentgamestatus) = soloGameLoop(currentgamestatus);
  }

  DrawAlways(cout,
             DataSuppliment(currentgamestatus, drawEndGameLoopGraphics));
  return gb;
}

Scoreboard::Score make_finalscore_from_game_session(double duration,
                                                    GameBoard gb) {
  Scoreboard::Score finalscore{};
  finalscore.score = gb.score;
  finalscore.win = hasWonOnGameboard(gb);
  finalscore.moveCount = MoveCountOnGameBoard(gb);
  finalscore.largestTile = gb.largestTile;
  finalscore.duration = duration;
  return finalscore;
}

void DoPostGameSaveStuff(Scoreboard::Score finalscore, competition_mode_t cm) {
  if (cm) {
    Statistics::CreateFinalScoreAndEndGameDataFile(cout, cin,
                                                   finalscore);
  }
}

} // namespace

// Helper to set stdin to non-blocking mode
void setNonBlockingStdin(bool enable) {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (enable)
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    else
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

void playGame(PlayGameFlag cont, GameBoard gb, ull userInput_PlaySize) {
  const auto is_this_a_new_game = (cont == PlayGameFlag::BrandNewGame);
  const auto is_competition_mode =
      (userInput_PlaySize == COMPETITION_GAME_BOARD_PLAY_SIZE);
  const auto bestScore = Statistics::load_game_best_score();

  if (is_this_a_new_game) {
    gb = GameBoard(userInput_PlaySize);
    addTileOnGameboard(gb);
  }

  auto startTime = chrono::high_resolution_clock::now();
  // Set stdin to non-blocking mode
  setNonBlockingStdin(true);

  // Main game loop
  auto loop_again = true;
    auto currentgamestatus = make_tuple(bestScore, is_competition_mode, gamestatus_t{}, gb);

    while (loop_again && ((mainmenustatus[FLAG_START_GAME] == true) || 
                          (mainmenustatus[FLAG_CONTINUE_GAME] == true))) {
      // Update duration before each frame
      auto now = chrono::high_resolution_clock::now();
      double elapsed = chrono::duration<double>(now - startTime).count();
      get<3>(currentgamestatus).duration = elapsed;
      // Redraw screen with updated timer
      DrawAlways(cout, DataSuppliment(currentgamestatus, drawGraphics));
      // Check for input (non-blocking)
      char c = 0;
      if (read(STDIN_FILENO, &c, 1) > 0) {
        // Process input
        Input::intendedmove_t player_intendedmove{};
        tie(player_intendedmove, get<2>(currentgamestatus)) =
            receive_agent_input(player_intendedmove, get<2>(currentgamestatus));
        tie(ignore, get<3>(currentgamestatus)) =
            process_agent_input(player_intendedmove, get<3>(currentgamestatus));
        
            // Process game logic
        auto [new_gamestatus, new_gb] = process_gamelogic(make_tuple(get<2>(currentgamestatus), get<3>(currentgamestatus)));
        get<2>(currentgamestatus) = new_gamestatus;
        get<3>(currentgamestatus) = new_gb;

        tie(loop_again, currentgamestatus) = soloGameLoop(currentgamestatus);
      }
      this_thread::sleep_for(chrono::milliseconds(100));
    }
    setNonBlockingStdin(false); // Reset stdin to blocking mode


    DrawAlways(cout, DataSuppliment(currentgamestatus, drawEndGameLoopGraphics));

    gb = get<3>(currentgamestatus);

  if (is_this_a_new_game) {
    const auto finalscore = make_finalscore_from_game_session(gb.duration, gb);
    DoPostGameSaveStuff(finalscore, is_competition_mode);
  }
}

void startGame() {
  PreGameSetup::SetUpNewGame();
}

/**
 * @brief Continue a previously saved game.
 * 
 * The ContinueOldGame function has been updated to accept a filename directly.
 * This allows the user to load a specific save file instead of only the last saved game.
 * 
 * @param filename The name of the file containing the saved game to load.
 */
void continueGame(const string& filename) {
  PreGameSetup::ContinueOldGame(filename);
}

/**
 * @brief Randomly removes two tiles from a GameBoard by setting their values to 0.
 *
 * This function uses a random number generator to select and remove two non-empty tiles 
 * from the given GameBoard by setting their values to 0. The process continues until 
 * exactly two tiles are removed.
 *
 * @param gb The GameBoard object from which tiles will be removed.
 */
void removeTiles(GameBoard& gb) {
    // Seed with a real random value, if available
    random_device rd;
    mt19937 gen(rd());

    auto& [playsize, tiles] = gb.gbda;
    uniform_int_distribution<> dis(0, tiles.size() - 1);

    int tiles_to_remove = 2;
    while (tiles_to_remove > 0) {
        int random_index = dis(gen);
        auto& tile = tiles[random_index];

        if (tile.value != 0) {  // Ensure it's not already an empty tile
            tile.value = 0;     // Remove the tile
            --tiles_to_remove;
        }
    }
}

} // namespace Game
