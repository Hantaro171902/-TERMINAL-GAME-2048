#include "global.hpp"
#include "color.hpp"
#include <iostream>
#include <sstream>

#ifdef _WIN32

void getKeypressDownInput(char &c) {
  cin >> c;
}

#else
#  include <termios.h>
#  include <unistd.h>

using namespace std;

char getch() {
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return (buf);
}

void getKeypressDownInput(char &c) {
  c = getch();
}

#endif

void pause_for_keypress() {
  char c{};
  getKeypressDownInput(c);
}

void wait_for_any_letter_input(istream &is) {
  char c;
  is >> c;
}

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
};

string secondsFormat(double sec) {
  double s = sec;
  int m = s / 60;
  s -= m * 60;
  int h = m / 60;
  m %= 60;
  s = (int)s;
  ostringstream oss;
  if (h) {
    oss << h << "h ";
  }
  if (m) {
    oss << m << "m ";
  }
  oss << s << "s";
  return oss.str();
}
