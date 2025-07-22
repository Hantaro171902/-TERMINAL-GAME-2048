#ifndef SCORES_H
#define SCORES_H

#include "global.hpp"
#include <iosfwd>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace Scoreboard {
struct Score {
  string name;
  ull score;
  bool win;
  ull largestTile;
  long long moveCount;
  double duration;
};

bool operator>(const Score &a, const Score &b);

using Scoreboard_t = vector<Score>;
using load_score_status_t = tuple<bool, Scoreboard_t>;

// List of scores read until "exhausted".
// Note: returns a tuple containing a vector<Score> of all read scores.
load_score_status_t loadFromFileScore(string filename);
void saveScore(Score finalscore);
} // namespace Scoreboard

istream &operator>>(istream &is, Scoreboard::Score &s);
ostream &operator<<(ostream &os, Scoreboard::Score &s);

#endif
