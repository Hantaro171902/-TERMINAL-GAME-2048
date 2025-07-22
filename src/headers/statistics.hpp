#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.hpp"
#include <iosfwd>
#include <string>
#include <tuple>

using namespace std;

namespace Scoreboard {
struct Score;
}

namespace Statistics {
struct total_game_stats_t {
  ull bestScore{};
  ull totalMoveCount{};
  int gameCount{};
  double totalDuration{};
  int winCount{};
};

using load_stats_status_t = tuple<bool, total_game_stats_t>;

load_stats_status_t loadFromFileStatistics(string filename);
ull load_game_best_score();
void saveEndGameStats(Scoreboard::Score finalscore);
void CreateFinalScoreAndEndGameDataFile(ostream &os, istream &is,
                                        Scoreboard::Score finalscore);
} // namespace Statistics

istream &operator>>(istream &is, Statistics::total_game_stats_t &s);
ostream &operator<<(ostream &os, Statistics::total_game_stats_t &s);

#endif
