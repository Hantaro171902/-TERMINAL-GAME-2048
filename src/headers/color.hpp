#ifndef COLOR_H
#define COLOR_H

#include <iostream>

using namespace std;

namespace Color {
  enum class Code {
    BOLD = 1,
    RESET = 0,
    BG_BLUE = 44,
    BG_DEFAULT = 49,
    BG_GREEN = 42,
    BG_RED = 41,
    FG_BLACK = 30,
    FG_BLUE = 34,
    FG_CYAN = 36,
    FG_DARK_GRAY = 90,
    FG_DEFAULT = 39,
    FG_GREEN = 32,
    FG_LIGHT_BLUE = 94,
    FG_LIGHT_CYAN = 96,
    FG_LIGHT_GRAY = 37,
    FG_LIGHT_GREEN = 92,
    FG_LIGHT_MAGENTA = 95,
    FG_LIGHT_RED = 91,
    FG_LIGHT_YELLOW = 93,
    FG_MAGENTA = 35,
    FG_RED = 31,
    FG_WHITE = 97,
    FG_YELLOW = 33,
  };

  class Modifier {
    string code_str;
    Code code;

  public:
    Modifier(Code pCode) : code_str("\033[" + to_string(static_cast<int>(pCode)) + "m") {}
    Modifier(int color256) : code_str("\033[38;5;" + to_string(color256) + "m") {} // For 256-color
    friend ostream &operator<<(ostream &os, const Modifier &mod) {
      return os << mod.code_str;
    }
  };

} // namespace Color

extern Color::Modifier bold_off;
extern Color::Modifier bold_on;
extern Color::Modifier def;
extern Color::Modifier white;
extern Color::Modifier yellow;
extern Color::Modifier orange;
extern Color::Modifier pink;
extern Color::Modifier red;
extern Color::Modifier green;
extern Color::Modifier blue;
extern Color::Modifier magenta;
extern Color::Modifier cyan;
extern Color::Modifier lightGray;
extern Color::Modifier darkGray;
extern Color::Modifier lightRed;
extern Color::Modifier lightGreen;
extern Color::Modifier lightYellow;
extern Color::Modifier lightBlue;
extern Color::Modifier lightMagenta;
extern Color::Modifier lightCyan;

#endif

