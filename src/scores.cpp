#include "scores.hpp"
#include "color.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>

using namespace std;

namespace {
using namespace Scoreboard;
bool generateFilefromScoreData(ostream &os, Score score) {
  os << score;
  return true;
}

Scoreboard_t generateScorefromFileData(istream &is) {
  Score tempscore{};
  Scoreboard_t scoreList{};
  while (is >> tempscore) {
    scoreList.push_back(tempscore);
  };
  return scoreList;
}

bool saveToFileScore(string filename, Score s) {
  ofstream os(filename, ios_base::app);
  return generateFilefromScoreData(os, s);
}

} // namespace

namespace Scoreboard {
bool operator>(const Score &a, const Score &b) {
  return a.score > b.score;
}

load_score_status_t loadFromFileScore(string filename) {
  ifstream scores(filename);
  if (scores) {
    Scoreboard_t scoreList = generateScorefromFileData(scores);
    sort(begin(scoreList), end(scoreList),
              greater<Score>{});
    return load_score_status_t{true, scoreList};
  }
  return load_score_status_t{false, Scoreboard_t{}};
}

void saveScore(Score finalscore) {
  saveToFileScore("../data/scores.txt", finalscore);
}

} // namespace Scoreboard

using namespace Scoreboard;

istream &operator>>(istream &is, Score &s) {
  is >> s.name >> s.score >> s.win >> s.moveCount >> s.largestTile >>
      s.duration;
  return is;
}

ostream &operator<<(ostream &os, Score &s) {
  os << "\n"
     << s.name << " " << s.score << " " << s.win << " " << s.moveCount << " "
     << s.largestTile << " " << s.duration;
  return os;
}
