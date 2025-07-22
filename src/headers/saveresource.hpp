#ifndef SAVERESOURCE_H
#define SAVERESOURCE_H

#include <string>
#include <tuple>

using namespace std;

namespace Game {
struct GameBoard;

namespace Saver {
void saveGamePlayState(GameBoard gb, const string& filename);

} // namespace Saver
} // namespace Game

#endif
