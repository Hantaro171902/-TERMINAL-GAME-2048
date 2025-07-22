#ifndef SCORESGRAPHICS_H
#define SCORESGRAPHICS_H

#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace Scoreboard {
namespace Graphics {
using scoreboard_display_data_t =
    tuple<string, string, string, string, string,
               string, string>;

using scoreboard_display_data_list_t = vector<scoreboard_display_data_t>;
string ScoreboardOverlay(scoreboard_display_data_list_t sbddl);

using finalscore_display_data_t =
    tuple<string, string, string, string>;
string EndGameStatisticsPrompt(finalscore_display_data_t finalscore);
} // namespace Graphics
} // namespace Scoreboard

#endif
