#ifndef GAMEPREGAMEMENU_H
#define GAMEPREGAMEMENU_H

#include <string>

using namespace std;

namespace Game {
namespace PreGameSetup {
    
void SetUpNewGame();
void ContinueOldGame(const string& filename);

} // namespace PreGameSetup
} // namespace Game

#endif
