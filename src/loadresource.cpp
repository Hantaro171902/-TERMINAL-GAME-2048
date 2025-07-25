#include "loadresource.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace Game {
namespace Loader {
namespace {

/**
 * @brief Counts the number of lines in a file until a line containing '[' is found.
 *
 * This function opens a file specified by the given filename and counts the number 
 * of lines until it encounters a line that contains the character '['. If the file 
 * cannot be opened, it prints an error message and returns -1.
 *
 * @param filename The name of the file to be read.
 * @return The number of lines read before encountering a line with '['. Returns -1 if the file cannot be opened.
 */
int GetLines(string filename) 
{
  ifstream stateFile(filename);
  if (!stateFile) {
      cerr << "Error. Cannot open the file: " << filename << endl;
      return -1; // Return -1 to indicate an error
  }

  string tempLine;
  int noOfLines = 0;

  while (getline(stateFile, tempLine)) {
      if (tempLine.find('[') != string::npos) {
          break; // Exit loop if '[' is found
      }
      noOfLines++;
  }

  return noOfLines;
}

/**
 * @brief Extracts tile data from a given input stream until a line containing '[' is encountered.
 *
 * This function reads lines from the provided input stream and extracts tile data 
 * formatted as comma-separated values. The process continues until either the maximum 
 * width (10 lines) is reached or a line containing the character '[' is found. When 
 * a line with '[' is encountered, the function stops reading further and processes 
 * the line up to, but not including, the '[' character.
 *
 * @param buf The input stream from which to read the tile data.
 * @return A vector of strings containing the extracted tile data.
 */
vector<string> get_file_tile_data(istream &buf) {
    vector<string> tempbuffer;
    enum { MAX_WIDTH = 10, MAX_HEIGHT = 10 };
    auto i{0};

    for (string tempLine; getline(buf, tempLine) && i < MAX_WIDTH; i++) {
        if (tempLine.find('[') != string::npos) {
            // Remove the '[' character and stop reading further
            tempLine = tempLine.substr(0, tempLine.find('['));
            istringstream temp_filestream(tempLine);
            auto j{0};
            for (string a_word; getline(temp_filestream, a_word, ',') && j < MAX_HEIGHT; j++) {
                tempbuffer.push_back(a_word);
            }
            break; // Stop the outer loop as the end of tile data is reached
        }

        istringstream temp_filestream(tempLine);
        auto j{0};
        for (string a_word; getline(temp_filestream, a_word, ',') && j < MAX_HEIGHT; j++) {
            tempbuffer.push_back(a_word);
        }
    }

    return tempbuffer;
}

vector<tile_t>
process_file_tile_string_data(vector<string> buf) {
  vector<tile_t> result_buf;
  auto tile_processed_counter{0};
  const auto prime_tile_data =
      [&tile_processed_counter](const string tile_data) {
        enum FieldIndex { IDX_TILE_VALUE, IDX_TILE_BLOCKED, MAX_NO_TILE_IDXS };
        array<int, MAX_NO_TILE_IDXS> tile_internal{};
        istringstream blocks(tile_data);
        auto idx_id{0};
        for (string temptiledata; getline(
                 blocks, temptiledata, ':') /*&& idx_id < MAX_NO_TILE_IDXS*/;
             idx_id++) {
          switch (idx_id) {
          case IDX_TILE_VALUE:
            get<IDX_TILE_VALUE>(tile_internal) = stoi(temptiledata);
            break;
          case IDX_TILE_BLOCKED:
            get<IDX_TILE_BLOCKED>(tile_internal) = stoi(temptiledata);
            break;
          default:
            cout << "ERROR: [tile_processed_counter: "
                      << tile_processed_counter
                      << "]: Read past MAX_NO_TILE_IDXS! (idx no:"
                      << MAX_NO_TILE_IDXS << ")\n";
          }
        }
        tile_processed_counter++;
        const unsigned long long tile_value =
            get<IDX_TILE_VALUE>(tile_internal);
        const bool tile_blocked = get<IDX_TILE_BLOCKED>(tile_internal);
        return tile_t{tile_value, tile_blocked};
      };
  transform(begin(buf), end(buf), back_inserter(result_buf),
                 prime_tile_data);
  return result_buf;
}

// TODO: Refactor and combine to one "gameboard" loader function!
tuple<bool, tuple<unsigned long long, long long>>
get_and_process_game_stats_string_data(istream &stats_file) {
  if (stats_file) {
    unsigned long long score;
    long long moveCount;
    for (string tempLine; getline(stats_file, tempLine);) {
      enum GameStatsFieldIndex {
        IDX_GAME_SCORE_VALUE,
        IDX_GAME_MOVECOUNT,
        MAX_NO_GAME_STATS_IDXS
      };
      istringstream line(tempLine);
      auto idx_id{0};
      for (string temp; getline(line, temp, ':'); idx_id++) {
        switch (idx_id) {
        case IDX_GAME_SCORE_VALUE:
          score = stoi(temp);
          break;
        case IDX_GAME_MOVECOUNT:
          moveCount = stoi(temp) - 1;
          break;
        default:
          // Error: No fields to process!
          break;
        }
      }
    }
    return make_tuple(true, make_tuple(score, moveCount));
  }
  return make_tuple(false, make_tuple(0, 0));
}

} // namespace

load_gameboard_status_t load_GameBoard_data_from_file(string filename) {
  ifstream stateFile(filename);
  if (stateFile) {
    const ull savedBoardPlaySize = GetLines(filename);
    const auto file_tile_data = get_file_tile_data(stateFile);
    const auto processed_tile_data =
        process_file_tile_string_data(file_tile_data);
    return make_tuple(true,
                           GameBoard(savedBoardPlaySize, processed_tile_data));
  }
  return make_tuple(false, GameBoard{});
}

// Output: [loadfile_ok_status, gameboard.score, gameboard.moveCount]
tuple<bool, tuple<unsigned long long, long long>>
load_game_stats_from_file(string filename) {
  ifstream stats(filename);
  return get_and_process_game_stats_string_data(stats);
}

/**
 * @brief Loads game data from a specified file into a GameBoard object.
 *
 * This function opens a file specified by the given filename, reads the game board data,
 * and initializes a GameBoard object with the read data. It first counts the number of lines 
 * until a line containing '[' is found to determine the size of the game board. Then, it reads
 * the tile data from the file, processes it, and updates the GameBoard object. Finally, it reads
 * the score and move count from the last relevant line containing these values.
 *
 * @param fileName The name of the file from which to read the game data.
 * @param gb The GameBoard object to be initialized with the read data.
 * @return true if the game data was successfully loaded; false otherwise.
 */
bool load_game(string fileName, GameBoard& gb)
{
  ifstream stateFile(fileName);
  if(!stateFile.is_open())
  {
    cerr << "Cannot open the file: " << fileName << endl;
    return false;
  }

  const ull savedBoardPlaySize = GetLines(fileName);
  const auto file_tile_data = get_file_tile_data(stateFile);
  const auto processed_tile_data = process_file_tile_string_data(file_tile_data);

  gb = GameBoard(savedBoardPlaySize, processed_tile_data);

  stateFile.clear();
  stateFile.seekg(0, ios::beg);

  string current_line;
  string last_line;

  while (getline(stateFile, current_line))
  {
    if (!current_line.empty() && current_line.front() == '[')
    {
      last_line = current_line;
      break;
    }
  }

  size_t opening_square_bracket = last_line.find('[');
  size_t colon = last_line.find(':');
  size_t closing_square_bracket = last_line.find(']');

  if (opening_square_bracket != string::npos && colon != string::npos && closing_square_bracket != string::npos)
  {
    string score = last_line.substr(opening_square_bracket + 1, colon - opening_square_bracket - 1);
    string moveCount = last_line.substr(colon + 1, closing_square_bracket - colon - 1);
    gb.score = stoull(score);
    gb.moveCount = stoll(moveCount) - 1;
  }

  return true;
}

} // namespace Loader

} // namespace Game
