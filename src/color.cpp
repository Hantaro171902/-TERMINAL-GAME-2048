// src/colors.cpp
#include "color.hpp"

Color::Modifier bold_off(Color::Code::RESET);
Color::Modifier bold_on(Color::Code::BOLD);
Color::Modifier def(Color::Code::FG_DEFAULT);
Color::Modifier white(Color::Code::FG_WHITE);
Color::Modifier yellow(Color::Code::FG_YELLOW);
Color::Modifier orange(208);            // Use 256-color code for orange
Color::Modifier pink(218);              // Use 256-color code for pink
Color::Modifier red(Color::Code::FG_RED);
Color::Modifier green(Color::Code::FG_GREEN);
Color::Modifier blue(Color::Code::FG_BLUE);
Color::Modifier magenta(Color::Code::FG_MAGENTA);
Color::Modifier cyan(Color::Code::FG_CYAN);
Color::Modifier lightGray(Color::Code::FG_LIGHT_GRAY);
Color::Modifier darkGray(Color::Code::FG_DARK_GRAY);
Color::Modifier lightRed(Color::Code::FG_LIGHT_RED);
Color::Modifier lightGreen(Color::Code::FG_LIGHT_GREEN);
Color::Modifier lightYellow(Color::Code::FG_LIGHT_YELLOW);
Color::Modifier lightBlue(Color::Code::FG_LIGHT_BLUE);
Color::Modifier lightMagenta(Color::Code::FG_LIGHT_MAGENTA);
Color::Modifier lightCyan(Color::Code::FG_LIGHT_CYAN);

