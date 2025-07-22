#ifndef STATISTICSGRAPHICS_H
#define STATISTICSGRAPHICS_H

#include <string>
#include <tuple>

using namespace std;

namespace Statistics {
namespace Graphics {
string AskForPlayerNamePrompt();
string MessageScoreSavedPrompt();

using total_stats_display_data_t =
    tuple<bool, string, string, string, string,
               string>;
string TotalStatisticsOverlay(total_stats_display_data_t tsdd);
} // namespace Graphics
} // namespace Statistics

#endif
