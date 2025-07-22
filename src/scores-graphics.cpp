#include "scores-graphics.hpp"
#include "color.hpp"
#include <array>
#include <iomanip>
#include <sstream>

using namespace std;

namespace Scoreboard {
namespace Graphics {

string ScoreboardOverlay(scoreboard_display_data_list_t sbddl) {
  constexpr auto no_save_text = "No saved scores.";
  const auto score_attributes_text = {
      "No.", "Name", "Score", "Won?", "Moves", "Largest Tile", "Duration"};
  //  constexpr auto num_of_score_attributes_text = 7;
  constexpr auto header_border_text =
      "┌─────┬────────────────────┬──────────┬──────┬───────┬──────────────┬──────────────┐";
  constexpr auto mid_border_text =
      "├─────┼────────────────────┼──────────┼──────┼───────┼──────────────┼──────────────┤";
  constexpr auto bottom_border_text =
      "└─────┴────────────────────┴──────────┴──────┴───────┴──────────────┴──────────────┘";
  constexpr auto score_title_text = "SCOREBOARD";
  constexpr auto divider_text = "──────────";
  constexpr auto sp = "  ";

  ostringstream str_os;

  str_os << green << bold_on << sp << score_title_text << bold_off << def
         << "\n";
  str_os << green << bold_on << sp << divider_text << bold_off << def << "\n";

  const auto number_of_scores = sbddl.size();
  if (number_of_scores) {
    str_os << sp << header_border_text << "\n";
    str_os << left;
    str_os << sp << "│ " << bold_on << begin(score_attributes_text)[0]
           << bold_off << " │ " << bold_on << setw(18)
           << begin(score_attributes_text)[1] << bold_off << " │ "
           << bold_on << setw(8) << begin(score_attributes_text)[2]
           << bold_off << " │ " << bold_on
           << begin(score_attributes_text)[3] << bold_off << " │ "
           << bold_on << begin(score_attributes_text)[4] << bold_off
           << " │ " << bold_on << begin(score_attributes_text)[5]
           << bold_off << " │ " << bold_on << setw(12)
           << begin(score_attributes_text)[6] << bold_off << " │"
           << "\n";
    str_os << right;
    str_os << sp << mid_border_text << "\n";

    const auto print_score_stat = [&](const scoreboard_display_data_t i) {
      str_os << sp << "│ " << setw(2) << get<0>(i) << ". │ "
             << left << setw(18) << get<1>(i) << right
             << " │ " << setw(8) << get<2>(i) << " │ " << setw(4)
             << get<3>(i) << " │ " << setw(5) << get<4>(i)
             << " │ " << setw(12) << get<5>(i) << " │ "
             << setw(12) << get<6>(i) << " │"
             << "\n";
    };

    // Use reference to avoid unnecessary copying of complex structures
    for (const auto& s : sbddl) {
      print_score_stat(s);
    }
    str_os << sp << bottom_border_text << "\n";
  } else {
    str_os << sp << no_save_text << "\n";
  }
  str_os << "\n\n";
  return str_os.str();
}

string EndGameStatisticsPrompt(finalscore_display_data_t finalscore) {
  ostringstream str_os;
  constexpr auto stats_title_text = "STATISTICS";
  constexpr auto divider_text = "──────────";
  constexpr auto sp = "  ";
  const auto stats_attributes_text = {
      "Final score:", "Largest Tile:", "Number of moves:", "Time taken:"};
  enum FinalScoreDisplayDataFields {
    IDX_FINAL_SCORE_VALUE,
    IDX_LARGEST_TILE,
    IDX_MOVE_COUNT,
    IDX_DURATION,
    MAX_NUM_OF_FINALSCOREDISPLAYDATA_INDEXES
  };
  const auto data_stats =
      array<string, MAX_NUM_OF_FINALSCOREDISPLAYDATA_INDEXES>{
          get<IDX_FINAL_SCORE_VALUE>(finalscore),
          get<IDX_LARGEST_TILE>(finalscore),
          get<IDX_MOVE_COUNT>(finalscore),
          get<IDX_DURATION>(finalscore)};

  ostringstream stats_richtext;
  stats_richtext << yellow << sp << stats_title_text << def << "\n";
  stats_richtext << yellow << sp << divider_text << def << "\n";

  auto counter{0};
  const auto populate_stats_info = [=, &counter,
                                    &stats_richtext](const string) {
    stats_richtext << sp << left << setw(19)
                   << begin(stats_attributes_text)[counter] << bold_on
                   << begin(data_stats)[counter] << bold_off << "\n";
    counter++;
  };

  for (const auto s : stats_attributes_text) {
    populate_stats_info(s);
  }

  str_os << stats_richtext.str();
  str_os << "\n\n";
  return str_os.str();
}

} // namespace Graphics

} // namespace Scoreboard
