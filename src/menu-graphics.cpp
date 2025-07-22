#include "menu-graphics.hpp"
#include "color.hpp"
#include "global.hpp"
#include <sstream>

using namespace std;
namespace Game {
namespace Graphics {
namespace Menu {

string MainMenuTitlePrompt() {
  constexpr auto greetings_text = "Welcome to ";
  constexpr auto gamename_text = "2048!";
  constexpr auto sp = "  ";

  ostringstream str_os;
  ostringstream title_richtext;
  title_richtext << bold_on << sp << greetings_text << blue << gamename_text
                 << def << bold_off << "\n";

  str_os << title_richtext.str();
  return str_os.str();
}

string MainMenuOptionsPrompt() {
  const auto menu_list_txt = {"1. Play a New Game", "2. Continue Previous Game",
                              "3. View Highscores and Statistics", "4. Exit"};
  constexpr auto sp = "        ";

  ostringstream str_os;

  str_os << "\n";
  for (const auto txt : menu_list_txt) {
    str_os << sp << txt << "\n";
  }
  str_os << "\n";

  return str_os.str();
}

string InputMenuErrorInvalidInputPrompt() {
  constexpr auto err_input_text = "Invalid input. Please try again.";
  constexpr auto sp = "  ";

  ostringstream str_os;
  ostringstream err_input_richtext;
  err_input_richtext << red << sp << err_input_text << def << "\n\n";

  str_os << err_input_richtext.str();
  return str_os.str();
}

string InputMenuPrompt() {
  constexpr auto prompt_choice_text = "Enter Choice: ";
  constexpr auto sp = "  ";

  ostringstream str_os;
  ostringstream prompt_choice_richtext;

  prompt_choice_richtext << sp << prompt_choice_text;

  str_os << prompt_choice_richtext.str();

  return str_os.str();
}

string MainMenuGraphicsOverlay(bool input_error_choice_invalid) {
  ostringstream str_os;
  DrawAlways(str_os, MainMenuTitlePrompt);
  DrawAlways(str_os, MainMenuOptionsPrompt);
  // Only outputs if there is an input error...
  DrawOnlyWhen(str_os, input_error_choice_invalid,
               InputMenuErrorInvalidInputPrompt);
  DrawAlways(str_os, InputMenuPrompt);
  return str_os.str();
}

} // namespace Menu
} // namespace Graphics
} // namespace Game
