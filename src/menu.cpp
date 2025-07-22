#include "menu.hpp"
#include "color.hpp"
#include "game-graphics.hpp"
#include "game.hpp"
#include "gameboard.hpp" // To iterate over all saved Games
#include "global.hpp"
#include "menu-graphics.hpp"
#include "scores-graphics.hpp"
#include "scores.hpp"
#include "statistics-graphics.hpp"
#include "statistics.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem> //To use filestream

using namespace std;

mainmenustatus_t mainmenustatus{};

/**
 * @brief Lists all saved game states in the specified directory.
 * 
 * This function scans the specified directory for regular files and returns their names
 * as a list of strings. It checks if the directory exists before attempting to list the files.
 * 
 * @param directory The directory to search for saved game state files.
 * @return vector<string> A vector containing the names of the saved game state files.
 */
vector<string> listSavedGameStates(const string& directory)
{
  vector<string> gameStates;

  if(!filesystem::exists(directory))
  {
    cout << "Directory does not exists." << endl;
    return gameStates;
  }

  for(auto& data : filesystem::directory_iterator(directory))
  {
    if(data.is_regular_file())
    {
      gameStates.push_back(data.path().filename().string());
    }
  }

  return gameStates;
}

/**
 * @brief Prompts the user to choose a game state from a list.
 * 
 * This function displays the available game states and prompts the user to select one.
 * If no game states are found, or if the user's choice is invalid, appropriate messages are displayed.
 * 
 * @param gamestate A vector containing the names of available game states.
 * @return string The name of the chosen game state, or an empty string if the choice is invalid.
 */
string chooseGameState(const vector<string>& gamestate)
{
  if(gamestate.empty())
  {
    cout << "No saved games found." << endl;
    return "";
  }

  cout << "Saved games are:" << endl;
  for (size_t i = 0; i < gamestate.size(); i++)
  {
    cout << i + 1 << ". " << gamestate[i] << endl;
  }

  unsigned int index;
  cout << "Choose game state:" << endl;
  cout << endl;
  cin >> index;

  if(index < 1 || index > gamestate.size())
  {
    cout << "Invalid choice." << endl;
    return "";
  }

  return gamestate[index - 1];
}

namespace {

bool FlagInputErrornousChoice{};

void startGame() {
  Game::startGame();
}

/**
 * @brief Continues a previously saved game.
 * 
 * This function prompts the user to choose a saved game state from a specified directory.
 * If a valid saved game state is selected, it continues the game using that state.
 * If no valid saved game state is selected, it notifies the user that the file is empty.
 * 
 * @note Changes in the new version:
 * - Added functionality to list and choose a saved game state from the specified directory.
 * - Validates the chosen game state file and continues the game using the selected file.
 * - Prints a message if the selected file is empty.
 */
void continueGame()
{
  string directory_state = "../data/SavedGameFiles/";
  string file_gb_state = chooseGameState(listSavedGameStates(directory_state));
  if (!file_gb_state.empty())
  {
    Game::continueGame(directory_state + file_gb_state);
  }
  else
  {
    cout << "The file is empty" << endl;
  }
}

Scoreboard::Graphics::scoreboard_display_data_list_t
make_scoreboard_display_data_list() {
  using namespace Scoreboard::Graphics;
  auto scoreList = Scoreboard::Scoreboard_t{};
  // bool loaded_scorelist;
  // Warning: Does not care if file exists or not!
  tie(ignore, scoreList) =
      Scoreboard::loadFromFileScore("../data/scores.txt");

  auto counter{1};
  const auto convert_to_display_list_t = [&counter](const Scoreboard::Score s) {
    const auto data_stats = make_tuple(
        to_string(counter), s.name, to_string(s.score),
        s.win ? "Yes" : "No", to_string(s.moveCount),
        to_string(s.largestTile), secondsFormat(s.duration));
    counter++;
    return data_stats;
  };

  auto scoreboard_display_list = scoreboard_display_data_list_t{};
  transform(begin(scoreList), end(scoreList),
                 back_inserter(scoreboard_display_list),
                 convert_to_display_list_t);
  return scoreboard_display_list;
};

Statistics::Graphics::total_stats_display_data_t
make_total_stats_display_data() {
  Statistics::total_game_stats_t stats;
  bool stats_file_loaded{};
  tie(stats_file_loaded, stats) =
      Statistics::loadFromFileStatistics("../data/statistics.txt");

  const auto tsdd = make_tuple(
      stats_file_loaded, to_string(stats.bestScore),
      to_string(stats.gameCount), to_string(stats.winCount),
      to_string(stats.totalMoveCount), secondsFormat(stats.totalDuration));
  return tsdd;
};

void showScores() {
  using namespace Game::Graphics;
  using namespace Scoreboard::Graphics;
  using namespace Statistics::Graphics;
  const auto sbddl = make_scoreboard_display_data_list();
  const auto tsdd = make_total_stats_display_data();

  clearScreen();
  DrawAlways(cout, AsciiArt2048);
  DrawAlways(cout, DataSuppliment(sbddl, ScoreboardOverlay));
  DrawAlways(cout, DataSuppliment(tsdd, TotalStatisticsOverlay));
  cout << flush;
  pause_for_keypress();
  ::Menu::startMenu();
}

void receive_input_flags(istream &in_os) {
  // Reset ErrornousChoice flag...
  FlagInputErrornousChoice = bool{};
  char c;
  in_os >> c;

  switch (c) {
  case '1':
    mainmenustatus[FLAG_START_GAME] = true;
    break;
  case '2':
    mainmenustatus[FLAG_CONTINUE_GAME] = true;
    break;
  case '3':
    mainmenustatus[FLAG_DISPLAY_HIGHSCORES] = true;
    break;
  case '4':
    mainmenustatus[FLAG_EXIT_GAME] = true;
    break;
  default:
    FlagInputErrornousChoice = true;
    break;
  }
}

void process_MainMenu() {
  if (mainmenustatus[FLAG_START_GAME]) {
    startGame();
  }
  if (mainmenustatus[FLAG_CONTINUE_GAME]) {
    continueGame();
  }
  if (mainmenustatus[FLAG_DISPLAY_HIGHSCORES]) {
    showScores();
  }
  if (mainmenustatus[FLAG_EXIT_GAME]) {
    exit(EXIT_SUCCESS);
  }
}

bool soloLoop() {
  // No choice in Menu selected, reset all flags...
  mainmenustatus = mainmenustatus_t{};
  clearScreen();
  DrawAlways(cout, Game::Graphics::AsciiArt2048);
  DrawAlways(cout,
             DataSuppliment(FlagInputErrornousChoice,
                            Game::Graphics::Menu::MainMenuGraphicsOverlay));
  receive_input_flags(cin);
  process_MainMenu();
  return FlagInputErrornousChoice;
}

/**
 * @brief Runs the endless loop until the game is exited.
 * 
 * This function continuously runs the solo game loop until the exit flag is set in the main menu status.
 * Initially, it sets the start menu flag to display the menu. If the start menu flag is set, it calls the solo loop
 * function to handle the menu interactions.
 * 
 * @note Changes in the new version:
 * - Added a condition to exit the loop when the exit flag is set in the main menu status.
 * - Integrated the menu start state and menu interaction within the loop.
 */
void endlessLoop() {
  // As long as the exit option is not selected in the menu
  while (!mainmenustatus[FLAG_EXIT_GAME])
  {
    // Initial state is Menu
    mainmenustatus[FLAG_START_MENU] = true;
    // If the menu flag is set, we enter...
    if (mainmenustatus[FLAG_START_MENU] == true)    
    {
      // ... the soloLoop() function, where we navigate the menu
      soloLoop();                         
    }                                               
  }
}

} // namespace

namespace Menu {

void startMenu() {
  endlessLoop();
}

} // namespace Menu
