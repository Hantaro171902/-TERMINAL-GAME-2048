#include "statistics.hpp"
#include "color.hpp"
#include "scores-graphics.hpp"
#include "scores.hpp"
#include "statistics-graphics.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>

using namespace std;

namespace Statistics {

namespace {

string receive_input_player_name(istream &is) {
  string name;
  is >> name;
  return name;
}

total_game_stats_t generateStatsFromInputData(istream &is) {
  total_game_stats_t stats;
  is >> stats;
  return stats;
}

bool generateFilefromStatsData(ostream &os, total_game_stats_t stats) {
  os << stats;
  return true;
}

bool saveToFileEndGameStatistics(string filename, total_game_stats_t s) {
  ofstream filedata(filename);
  return generateFilefromStatsData(filedata, s);
}

Scoreboard::Graphics::finalscore_display_data_t
make_finalscore_display_data(Scoreboard::Score finalscore) {
  const auto fsdd = make_tuple(
      to_string(finalscore.score), to_string(finalscore.largestTile),
      to_string(finalscore.moveCount), secondsFormat(finalscore.duration));
  return fsdd;
};

} // namespace

load_stats_status_t loadFromFileStatistics(string filename) {
  ifstream statistics(filename);
  if (statistics) {
    total_game_stats_t stats = generateStatsFromInputData(statistics);
    return load_stats_status_t{true, stats};
  }
  return load_stats_status_t{false, total_game_stats_t{}};
}

ull load_game_best_score() {
  total_game_stats_t stats;
  bool stats_file_loaded{};
  ull tempscore{0};
  tie(stats_file_loaded, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  if (stats_file_loaded) {
    tempscore = stats.bestScore;
  }
  return tempscore;
}

void saveEndGameStats(Scoreboard::Score finalscore) {
  total_game_stats_t stats;
  // Need some sort of stats data values only.
  // No need to care if file loaded successfully or not...
  tie(ignore, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  stats.bestScore =
      stats.bestScore < finalscore.score ? finalscore.score : stats.bestScore;
  stats.gameCount++;
  stats.winCount = finalscore.win ? stats.winCount + 1 : stats.winCount;
  stats.totalMoveCount += finalscore.moveCount;
  stats.totalDuration += finalscore.duration;

  saveToFileEndGameStatistics("../data/statistics.txt", stats);
}

void CreateFinalScoreAndEndGameDataFile(ostream &os, istream &is,
                                        Scoreboard::Score finalscore) {
  const auto finalscore_display_data = make_finalscore_display_data(finalscore);
  DrawAlways(os, DataSuppliment(finalscore_display_data,
                                Scoreboard::Graphics::EndGameStatisticsPrompt));

  DrawAlways(os, Graphics::AskForPlayerNamePrompt);
  const auto name = receive_input_player_name(is);
  finalscore.name = name;

  Scoreboard::saveScore(finalscore);
  saveEndGameStats(finalscore);
  DrawAlways(os, Graphics::MessageScoreSavedPrompt);
}

} // namespace Statistics

using namespace Statistics;

istream &operator>>(istream &is, total_game_stats_t &s) {
  is >> s.bestScore >> s.gameCount >> s.winCount >> s.totalMoveCount >>
      s.totalDuration;
  return is;
}

ostream &operator<<(ostream &os, total_game_stats_t &s) {
  os << s.bestScore << "\n"
     << s.gameCount << "\n"
     << s.winCount << "\n"
     << s.totalMoveCount << "\n"
     << s.totalDuration;
  return os;
}
