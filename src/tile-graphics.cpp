#include "tile-graphics.hpp"
#include "color.hpp"
#include "tile.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

namespace Game {
namespace {

Color::Modifier tileColor(ull value) {
  vector<Color::Modifier> colors{lightBlue, magenta, yellow, red, green, white,
                                      red, green, cyan, blue, magenta};
  int log = log2(value);
  int index = log < 12 ? log - 1 : 10;

  return colors[index];
}
} // namespace

string drawTileString(tile_t currentTile) {
  ostringstream tile_richtext;
  if (!currentTile.value) {
    tile_richtext << "    ";
  } else {
    tile_richtext << tileColor(currentTile.value) << bold_on << setw(4)
                  << currentTile.value << bold_off << def;
  }
  return tile_richtext.str();
}
} // namespace Game
