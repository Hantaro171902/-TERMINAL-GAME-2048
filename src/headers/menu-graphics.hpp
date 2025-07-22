#ifndef MENUGRAPHICS_H
#define MENUGRAPHICS_H

#include <string>

using namespace std;

namespace Game {
namespace Graphics {
namespace Menu {
string MainMenuTitlePrompt();
string MainMenuOptionsPrompt();
string InputMenuErrorInvalidInputPrompt();
string InputMenuPrompt();
string MainMenuGraphicsOverlay(bool input_error_choice_invalid);
} // namespace Menu
} // namespace Graphics
} // namespace Game

#endif
