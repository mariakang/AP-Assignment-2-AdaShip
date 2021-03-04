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

/** Converts the given column number to its alphabetic representation. */
string GameController::columnToString(int column) {
  const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string alpha = "";
  if (column > 26) {
    alpha += alphabet[(column - 1) / 26 - 1];
  }
  alpha += alphabet[(column - 1) % 26];
  return alpha;
}

/** Returns a string representation of the given Coordinate. */
string GameController::coordinateToString(Coordinate c) {
  return columnToString(c.column()) + to_string(c.row());
}

/** Returns a randomly generated number between 1 and 'upperBound' inclusive. */
int GameController::randomNumber(int upperBound) {
  // initialize random seed (required for the numbers to be generated randomly)
  srand (time(NULL));
  // return a random number between 1 and 'upperBound'
  return rand() % upperBound + 1;
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

/** 
 * Fires a torpedo at the given Coordinate on the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
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

/** 
 * Fires a torpedo at a randomly generated Coordinate on the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
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

/** 
 * Places the boat with the given boatId on the given player's board, at the given
 * Coordinate and orientation.
 *
 * Returns whether or not the action was completed successfully.
 */
bool GameController::placeBoat(Player& player, int boatId, Coordinate c, bool vertical) {
  // if the coordinate is invalid, return false
  if (!isValidCoordinate(c)) {
    return false;
  }
  // get the board, square and boat relating to the parameters
  Board& board = player.board();
  BoardSquare& square = board.getSquare(c);
  Boat& boat = player.getBoat(boatId);
  // if the boat length exceeds the maximum that can be placed in the square
  // (for the given orientation) return false
  if ((vertical && square.maxBoatLengthVertical() < boat.length())
        || (!vertical && square.maxBoatLengthHorizontal() < boat.length())) {
    return false;
  }
  // if the boat already has a start Coordinate (which isn't equal to (0, 0))
  // then the boat needs to be removed from the board first
  if (boat.isPlaced()) {
    removeBoat(player, boat);
  }

  boat.setStart(c);
  boat.setVertical(vertical);

  if (vertical) {
    // iterate over the column the boat is being placed in,
    // from the top of the board to the end of the boat
    for (int i = 1; i < c.row() + boat.length(); i++) {
      Coordinate ci(i, c.column());
      BoardSquare& squarei = board.getSquare(ci);
      // if we haven't reached the start of the boat, just
      // update the maxBoatLengthVertical (to be the minimum of
      // its existing value, and the number of spaces before
      // the start of the newly placed boat)
      if (i < c.row()) {
        int currentMaxV = squarei.maxBoatLengthVertical();
        int newMaxV = c.row() - i;
        if (newMaxV < currentMaxV) {
          squarei.setMaxBoatLengthVertical(newMaxV);
        }
      // if we've reached the start of the boat, update both
      // maxBoatLengthVertical and maxBoatLengthHorizontal to 0,
      // set the boatId, and update the maxBoatLengthHorizontal
      // values of squares to the left of the current one
      } else {
        squarei.setMaxBoatLengthVertical(0);
        squarei.setMaxBoatLengthHorizontal(0);
        squarei.setBoatId(boatId);
        // iterate over squares to the left of this one
        for (int j = 1; j < c.column(); j++) {
          Coordinate cij(i, j);
          BoardSquare& squareij = board.getSquare(cij);
          // update the maxBoatLengthHorizontal (to be the minimum
          // of its existing value, and the number of spaces
          // before 'squarei')
          int currentMaxH = squareij.maxBoatLengthHorizontal();
          int newMaxH = c.column() - j;
          if (newMaxH < currentMaxH) {
            squareij.setMaxBoatLengthHorizontal(newMaxH);
          }
        }
      }
    }
  } else {
    // the boat must be placed horizontally
    // iterate over the row the boat is being placed in,
    // from the left of the board to the end of the boat
    for (int j = 1; j < c.column() + boat.length(); j++) {
      Coordinate cj(c.row(), j);
      BoardSquare& squarej = board.getSquare(cj);
      // if we haven't reached the start of the boat, just
      // update the maxBoatLengthHorizontal (to be the minimum of
      // its existing value, and the number of spaces before
      // the start of the newly placed boat)
      if (j < c.column()) {
        int currentMaxH = squarej.maxBoatLengthHorizontal();
        int newMaxH = c.column() - j;
        if (newMaxH < currentMaxH) {
          squarej.setMaxBoatLengthHorizontal(newMaxH);
        }
      // if we've reached the start of the boat, update both
      // maxBoatLengthVertical and maxBoatLengthHorizontal to 0,
      // set the boatId, and update the maxBoatLengthVertical
      // values of squares above the current one
      } else {
        squarej.setMaxBoatLengthVertical(0);
        squarej.setMaxBoatLengthHorizontal(0);
        squarej.setBoatId(boatId);
        // iterate over squares above this one
        for (int i = 1; i < c.row(); i++) {
          Coordinate cij(i, j);
          BoardSquare& squareij = board.getSquare(cij);
          // update the maxBoatLengthVertical (to be the minimum
          // of its existing value, and the number of spaces
          // above 'squarej')
          int currentMaxV = squareij.maxBoatLengthVertical();
          int newMaxV = c.row() - i;
          if (newMaxV < currentMaxV) {
            squareij.setMaxBoatLengthVertical(newMaxV);
          }
        }
      }
    }
  }
  return true;
}

/** 
 * Removes the given boat from the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
bool GameController::removeBoat(Player& player, Boat& boat) {
  // if the boat hasn't been placed, return false.
  if (!boat.isPlaced()) {
    return false;
  }

  // update the board
  Board& board = player.board();
  Coordinate start = boat.start();
  // if the boat is placed vertically, starting from the bottom
  // of the boat and working up through the column until another
  // boat is encountered, update the squares
  if (boat.vertical()) {
    // get the maxBoatLengthVertical for the square immediately
    // underneath the boat (or 0 if at the bottom of the board)
    int lastBoatRow = start.row() + boat.length() - 1;
    int maxLengthV = 0;
    if (lastBoatRow < board.rows()) {
      Coordinate c(lastBoatRow + 1, start.column());
      maxLengthV = board.getSquare(c).maxBoatLengthVertical();
    }
    // as we move up through the board, increment maxLengthV
    // until we hit another boat
    for (int i = lastBoatRow; i >= 1; i--) {
      maxLengthV++;
      Coordinate ci(i, start.column());
      BoardSquare& squarei = board.getSquare(ci);
      // update the maxBoatLengthVertical
      squarei.setMaxBoatLengthVertical(maxLengthV);
      // if we're currently inside the boat, update the square's
      // boatId to -1, and update the maxBoatLengthHorizontal
      // for this square and the squares to the left
      if (i >= start.row()) {
        squarei.setBoatId(-1);
        // update squares to the left

      // if we've gone past the start, check if we've hit another
      // boat, and exit if we have
      } else {
        if (squarei.boatId() > -1) {
          break;
        }
      }
    }
  // if the boat is placed horizontally, starting from the end
  // of the boat and working back through the row until another
  // boat is encountered, update the squares
  } else {
    // get the maxBoatLengthHorizontal for the square immediately
    // to the right of the boat (or 0 if at the end of the board)
    int lastBoatCol = start.column() + boat.length() - 1;
    int maxLengthH = 0;
    if (lastBoatCol < board.columns()) {
      Coordinate c(start.row(), lastBoatCol + 1);
      maxLengthH = board.getSquare(c).maxBoatLengthHorizontal();
    }
    // as we move left through the board, increment maxLengthH
    // until we hit another boat
    for (int j = lastBoatCol; j >= 1; j--) {
      maxLengthH++;
      Coordinate cj(start.row(), j);
      BoardSquare& squarej = board.getSquare(cj);
      // update the maxBoatLengthHorizontal
      squarej.setMaxBoatLengthHorizontal(maxLengthH);
      // if we're currently inside the boat, update the square's
      // boatId to -1, and update the maxBoatLengthVertical
      // for this square and the squares above it
      if (j >= start.column()) {
        squarej.setBoatId(-1);
        // update squares above

      // if we've gone past the start, check if we've hit another
      // boat, and exit if we have
      } else {
        if (squarej.boatId() > -1) {
          break;
        }
      }
    }
  }

  // reset the boat's start to (0, 0) (which isn't on the board)
  Coordinate origin;
  boat.setStart(origin);
  // note: we don't care about resetting the orientation; it's only
  // needed after the boat has been placed (and it will automatically
  // be set again when the boat is replaced)
  return true;
}
