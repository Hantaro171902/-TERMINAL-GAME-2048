#include "game-graphics.hpp"
#include "color.hpp"
#include "global.hpp"
#include <array>
#include <iomanip>
#include <sstream>

using namespace std;

namespace Game {
namespace Graphics {
string AsciiArt2048() {
  constexpr auto title_card_2048 = R"(
   /\\\\\\\\\          /\\\\\\\                /\\\         /\\\\\\\\\
  /\\\///////\\\      /\\\/////\\\            /\\\\\       /\\\///////\\\
  \///      \//\\\    /\\\    \//\\\         /\\\/\\\      \/\\\     \/\\\
             /\\\/    \/\\\     \/\\\       /\\\/\/\\\      \///\\\\\\\\\/
           /\\\//      \/\\\     \/\\\     /\\\/  \/\\\       /\\\///////\\\
         /\\\//         \/\\\     \/\\\   /\\\\\\\\\\\\\\\\   /\\\      \//\\\
        /\\\/            \//\\\    /\\\   \///////////\\\//   \//\\\      /\\\
        /\\\\\\\\\\\\\\\   \///\\\\\\\/              \/\\\      \///\\\\\\\\\/
        \///////////////      \///////                \///         \/////////
  )";


  ostringstream title_card_richtext;
  title_card_richtext << lightCyan << bold_on << title_card_2048 << bold_off << def;
  title_card_richtext << "\n\n\n";
  return title_card_richtext.str();
}

string BoardInputPrompt() {
  const auto board_size_prompt_text = {
      "(NOTE: Scores and statistics will be saved only for the 4x4 gameboard)\n",
      "Enter gameboard size - (Enter '0' to go back): "};
  constexpr auto sp = "  ";
  ostringstream board_size_prompt_richtext;
  board_size_prompt_richtext
      << bold_on << sp << begin(board_size_prompt_text)[0] << sp
      << begin(board_size_prompt_text)[1] << bold_off;
  return board_size_prompt_richtext.str();
}

string YouWinPrompt() {
  constexpr auto win_game_text = "You win! Congratulations!";
  constexpr auto sp = "  ";
  ostringstream win_richtext;
  win_richtext << green << bold_on << sp << win_game_text << def << bold_off
               << "\n\n\n";
  return win_richtext.str();
}

string GameOverPrompt() {
  constexpr auto lose_game_text = "Game over! You lose.";
  constexpr auto sp = "  ";
  ostringstream lose_richtext;
  lose_richtext << red << bold_on << sp << lose_game_text << def << bold_off
                << "\n\n\n";
  return lose_richtext.str();
}

string EndOfEndlessPrompt() {
  constexpr auto endless_mode_text =
      "End of endless mode! Thank you for playing!";
  constexpr auto sp = "  ";
  ostringstream endless_mode_richtext;
  endless_mode_richtext << red << bold_on << sp << endless_mode_text << def
                        << bold_off << "\n\n\n";
  return endless_mode_richtext.str();
}

string QuestionEndOfWinningGamePrompt() {
  constexpr auto win_but_what_next =
      "You Won! Continue playing current game? [y/n]";
  constexpr auto sp = "  ";
  ostringstream win_richtext;
  win_richtext << green << bold_on << sp << win_but_what_next << def << bold_off
               << ": ";
  return win_richtext.str();
}

string GameStateNowSavedPrompt() {
  constexpr auto state_saved_text =
      "The game has been saved. Feel free to take a break.";
  constexpr auto sp = "  ";
  ostringstream state_saved_richtext;
  state_saved_richtext << green << bold_on << sp << state_saved_text << def
                       << bold_off << "\n\n";
  return state_saved_richtext.str();
}

string GameBoardNoSaveErrorPrompt() {
  constexpr auto no_save_found_text =
      "No saved game found. Starting a new game.";
  constexpr auto sp = "  ";
  ostringstream no_save_richtext;
  no_save_richtext << red << bold_on << sp << no_save_found_text << def
                   << bold_off << "\n\n";
  return no_save_richtext.str();
}

string InvalidInputGameBoardErrorPrompt() {
  constexpr auto invalid_prompt_text = "Invalid input. Please try again.";
  constexpr auto sp = "  ";
  ostringstream invalid_prompt_richtext;
  invalid_prompt_richtext << red << sp << invalid_prompt_text << def << "\n\n";
  return invalid_prompt_richtext.str();
}

string BoardSizeErrorPrompt() {
  const auto invalid_prompt_text = {
      "Invalid input. Gameboard size should range from ", " to ", "."};
  //  constexpr auto num_of_invalid_prompt_text = 3;
  constexpr auto sp = "  ";
  ostringstream error_prompt_richtext;
  error_prompt_richtext << red << sp << begin(invalid_prompt_text)[0]
                        << MIN_GAME_BOARD_PLAY_SIZE
                        << begin(invalid_prompt_text)[1]
                        << MAX_GAME_BOARD_PLAY_SIZE
                        << begin(invalid_prompt_text)[2] << def << "\n\n";
  return error_prompt_richtext.str();
}

/**
 * @brief Generates a string prompt listing the available input commands.
 * 
 * This function creates a formatted string that lists the available input commands for the game.
 * The commands include movements (Up, Left, Down, Right), saving the game, and returning to the menu.
 * The prompt is formatted with indentation for readability.
 * 
 * 
 * 
 * @return string A formatted string containing the list of input commands.
 */
string InputCommandListPrompt() {
  constexpr auto sp = "  ";
  const auto input_commands_list_text = {
      "W K ↑ : Up", 
      "A H ← : Left", 
      "S J ↓ : Down",
      "D L → : Right", 
      "Z P   : Save", 
      "M     : Return to menu"};
  ostringstream str_os;
  for (const auto txt : input_commands_list_text) {
    str_os << sp << txt << "\n";
  }
  return str_os.str();
}

string EndlessModeCommandListPrompt() {
  constexpr auto sp = "  ";
  const auto endless_mode_list_text = {"X => Quit Endless Mode"};
  ostringstream str_os;
  for (const auto txt : endless_mode_list_text) {
    str_os << sp << txt << "\n";
  }
  return str_os.str();
}

string InputCommandListFooterPrompt() {
  constexpr auto sp = "  ";
  const auto input_commands_list_footer_text = {
      "", "Press the keys to start and continue.", "\n"};
  ostringstream str_os;
  for (const auto txt : input_commands_list_footer_text) {
    str_os << sp << txt << "\n";
  }
  return str_os.str();
}

string GameScoreBoardBox(scoreboard_display_data_t scdd) {
  ostringstream str_os;
  constexpr auto score_text_label = "SCORE:";
  constexpr auto bestscore_text_label = "BEST SCORE:";
  constexpr auto moves_text_label = "MOVES:";
  constexpr auto time_text_label = "TIME:";

  // * border padding: vvv
  // | l-outer: 2, r-outer: 0
  // | l-inner: 1, r-inner: 1
  // * top border / bottom border: vvv
  // | tl_corner + horizontal_sep + tr_corner = length: 1 + 27 + 1
  // | bl_corner + horizontal_sep + br_corner = length: 1 + 27 + 1
  enum {
    UI_SCOREBOARD_SIZE = 27,
    UI_BORDER_OUTER_PADDING = 2,
    UI_BORDER_INNER_PADDING = 1
  }; // length of horizontal board - (corners + border padding)
  constexpr auto border_padding_char = ' ';
  constexpr auto vertical_border_pattern = "│";
  constexpr auto top_board =
      "┌───────────────────────────┐"; // Multibyte character set
  constexpr auto bottom_board =
      "└───────────────────────────┘"; // Multibyte character set
  const auto outer_border_padding =
      string(UI_BORDER_OUTER_PADDING, border_padding_char);
  const auto inner_border_padding =
      string(UI_BORDER_INNER_PADDING, border_padding_char);
  const auto inner_padding_length =
      UI_SCOREBOARD_SIZE - (string{inner_border_padding}.length() * 2);

  enum ScoreBoardDisplayDataFields {
    IDX_COMPETITION_MODE,
    IDX_GAMEBOARD_SCORE,
    IDX_BESTSCORE,
    IDX_MOVECOUNT,
    IDX_TIME_STR, 
    MAX_SCOREBOARDDISPLAYDATA_INDEXES
  };

  const auto competition_mode = get<IDX_COMPETITION_MODE>(scdd);
  const auto gameboard_score = get<IDX_GAMEBOARD_SCORE>(scdd);
  const auto temp_bestscore = get<IDX_BESTSCORE>(scdd);
  const auto movecount = get<IDX_MOVECOUNT>(scdd);
  const auto time_str = get<IDX_TIME_STR>(scdd); // Placeholder for time string

  str_os << outer_border_padding << top_board << "\n";
  str_os << outer_border_padding << vertical_border_pattern
         << inner_border_padding << bold_on << score_text_label << bold_off
         << string(inner_padding_length -
                            string{score_text_label}.length() -
                            gameboard_score.length(),
                        border_padding_char)
         << gameboard_score << inner_border_padding << vertical_border_pattern
         << "\n";
  if (competition_mode) {
    str_os << outer_border_padding << vertical_border_pattern
           << inner_border_padding << bold_on << bestscore_text_label
           << bold_off
           << string(inner_padding_length -
                              string{bestscore_text_label}.length() -
                              temp_bestscore.length(),
                          border_padding_char)
           << temp_bestscore << inner_border_padding << vertical_border_pattern
           << "\n";
  }
  str_os << outer_border_padding << vertical_border_pattern
         << inner_border_padding << bold_on << moves_text_label << bold_off
         << string(inner_padding_length -
                            string{moves_text_label}.length() -
                            movecount.length(),
                        border_padding_char)
         << movecount << inner_border_padding << vertical_border_pattern
         << "\n";
  // Add time string to the scoreboard
  str_os << outer_border_padding << vertical_border_pattern
         << inner_border_padding << bold_on << time_text_label << bold_off
         << string(inner_padding_length -
                            string{time_text_label}.length() -
                            time_str.length(),
                        border_padding_char)
         << time_str << inner_border_padding << vertical_border_pattern
         << "\n";

  str_os << outer_border_padding << bottom_board << "\n \n";
  return str_os.str();
}

string GameScoreBoardOverlay(scoreboard_display_data_t scdd) {
  ostringstream str_os;
  DrawAlways(str_os, DataSuppliment(scdd, GameScoreBoardBox));
  return str_os.str();
}

string GameEndScreenOverlay(end_screen_display_data_t esdd) {
  enum EndScreenDisplayDataFields {
    IDX_FLAG_WIN,
    IDX_FLAG_ENDLESS_MODE,
    MAX_ENDSCREENDISPLAYDATA_INDEXES
  };
  const auto did_win = get<IDX_FLAG_WIN>(esdd);
  const auto is_endless_mode = get<IDX_FLAG_ENDLESS_MODE>(esdd);

  ostringstream str_os;
  const auto standardWinLosePrompt = [=] {
    ostringstream str_os;
    DrawOnlyWhen(str_os, did_win, YouWinPrompt);
    // else..
    DrawOnlyWhen(str_os, !did_win, GameOverPrompt);
    return str_os.str();
  };
  DrawOnlyWhen(str_os, !is_endless_mode, standardWinLosePrompt);
  // else..
  DrawOnlyWhen(str_os, is_endless_mode, EndOfEndlessPrompt);
  return str_os.str();
}

string GameInputControlsOverlay(input_controls_display_data_t gamestatus) {
  const auto is_in_endless_mode = get<0>(gamestatus);
  const auto is_in_question_mode = get<1>(gamestatus);
  ostringstream str_os;
  const auto InputControlLists = [=] {
    ostringstream str_os;
    DrawAlways(str_os, Graphics::InputCommandListPrompt);
    DrawOnlyWhen(str_os, is_in_endless_mode,
                 Graphics::EndlessModeCommandListPrompt);
    DrawAlways(str_os, Graphics::InputCommandListFooterPrompt);
    return str_os.str();
  };
  // When game is paused to ask a question, hide regular inut prompts..
  DrawOnlyWhen(str_os, !is_in_question_mode, InputControlLists);
  return str_os.str();
}

} // namespace Graphics
} // namespace Game
