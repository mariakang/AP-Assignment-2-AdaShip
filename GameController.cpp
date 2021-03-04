#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <time.h>

using namespace std;

#include "Config.h"
#include "Coordinate.h"
#include "GameController.h"
#include "Player.h"

/** Constructor function for gameController. */
GameController::GameController(Config config) {
  // set number of rows and columns
  rows_ = config.rows();
  columns_ = config.columns();
  // populate an 'alpha to column number' lookup map
  const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (int i = 1; i <= columns_; i++) {
    alphaLookup_[columnToString(i)] = i;
  }
}

string GameController::columnToString(int column) {
  const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string alpha = "";
  if (column > 26) {
    alpha += alphabet[(column - 1) / 26 - 1];
  }
  alpha += alphabet[(column - 1) % 26];
  return alpha;
}

string GameController::coordinateToString(Coordinate c) {
  return columnToString(c.column()) + to_string(c.row());
}

/** 
 * Returns a coordinate corresponding to the given string.
 *
 * If the string is not a valid coordinate, then (0, 0) is returned.
 */
Coordinate GameController::getCoordinate(string s) {
  // initialise an index to iterate over the string
  int index = 0;
  // initialise a default coordinate
  Coordinate coordinate;
  // calculate the alphabetic portion of the string
  string alphabetic = "";
  while (index < s.length() && isalpha(s[index])) {
    alphabetic += toupper(s[index]);
    index++;
  }
  // if the alphabetic portion is not in the lookup map, return (0, 0)
  if (alphaLookup_.find(alphabetic) == alphaLookup_.end()) {
    return coordinate;
  }
  // otherwise, look up the column number it represents
  int column = alphaLookup_[alphabetic];

  // calculate the numeric part of the string
  string numeric = "";
  while (index < s.length() && isdigit(s[index])) {
    numeric += s[index];
    index++;
  }
  // if we haven't reached the end of the string, return (0, 0)
  if (index < s.length()) {
    return coordinate;
  }
  // find the row corresponding to the numeric part of the string
  int row = stoi(numeric);

  // if the row number exceeds the total number of rows, return (0, 0)
  if (row > rows_) {
    return coordinate;
  }

  // update and return coordinate
  coordinate.setRow(row);
  coordinate.setColumn(column);
  return coordinate;
}

bool GameController::torpedo(Player& player, Coordinate c) {
  if (!isValidCoordinate(c)) {
    return false;
  }
  Board& board = player.board();
  BoardSquare& square = board.getSquare(c);
  if (square.torpedoed()) {
    return false;
  }
  square.setTorpedoed(true);
  board.decrementRemainingTargets();
  if (square.boatId() < 0) {
    cout << "Miss\n";
  } else {
    cout << "Hit!\n";
    Boat& boat = player.getBoat(square.boatId());
    boat.hit();
    if (boat.isSunk()) {
      cout << boat.name() << " has been sunk!\n";
      player.decrementSurvivingBoats();
    }
  }
  if (square.hasMine()) {
    cout << "Mine!\n";
    for (int i = c.row() - 1; i <= c.row() + 1; i++) {
      for (int j = c.column() - 1; j <= c.column() + 1; j++) {
        Coordinate neighbour(i, j);
        torpedo(player, neighbour);
      }
    }
  }
  return true;
}

bool GameController::torpedoRandom(Player& player) {
  Board& board = player.board();
  int target = randomNumber(board.remainingTargets());
  int counter = 0;
  for (int i = 1; i <= rows_; i++) {
    for (int j = 1; j <= columns_; j++) {
      Coordinate c(i, j);
      if (!board.getSquare(c).torpedoed()) {
        counter++;
        if (counter == target) {
          cout << "Targeting " << coordinateToString(c) << "... ";
          return torpedo(player, c);
        }
      }
    }
  }
  return false;
}

int GameController::randomNumber(int upperBound) {
  // initialize random seed (required for the numbers to be generated randomly)
  srand (time(NULL));
  // return a random number between 1 and 'upperBound'
  return rand() % upperBound + 1;
}
