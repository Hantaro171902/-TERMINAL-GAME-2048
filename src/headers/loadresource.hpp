#ifndef LOADRESOURCE_H
#define LOADRESOURCE_H

#include <string>
#include <tuple>

using namespace std;

namespace Game {
using load_gameboard_status_t = tuple<bool, struct GameBoard>;

namespace Loader {
load_gameboard_status_t load_GameBoard_data_from_file(string filename);

// Output: {[loadfile_ok_status], [decltype(gameboard.score)],
// [decltype(gameboard.moveCount)]}
tuple<bool, tuple<unsigned long long, long long>>
load_game_stats_from_file(string filename);
bool load_game(string filename, GameBoard& gb);

} // namespace Loader
} // namespace Game

#endif
