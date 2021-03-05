#include <iostream>
#include <string>

using namespace std;

#include "Board.h"
#include "BoardPrinter.h"
#include "Boat.h"
#include "Config.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"
#include "Player.h"

// "\033[<code>m" changes the format based on the given <code>.
// The code may consist of several codes, separated by ';'.
//    0 -> reset
//    1 -> bold
//    30 -> black
//    31 -> red
//    32 -> green
//    34 -> blue
//    37 -> white
//    41 -> red background
//    46 -> cyan background
//    100 -> bright black background
//    104 -> bright blue background
 
#define WAVE "\033[34;104m ~ \033[0m"
#define SPLASH "\033[1;104m * \033[0m"
#define BOAT_PREFIX "\033[30;100m " // white text on red
#define HIT_BOAT_PREFIX "\033[37;41m " // black text on grey ('bright black')
#define BOAT_SUFFIX " \033[0m" // resets the format

void BoardPrinter::printBoardBoats(Player player) {
  // get player's board
  Board& board = player.board();
  int rows = board.rows();
  int columns = board.columns();
  // print the header row
  printHeader(columns);
  // iterate over the board
  for (int i = 1; i <= rows; i++) {
    // print the row number
    if (i <= 9) {
      cout << " ";
    }
    cout << i << " |";
    // iterate over the columns
    for (int j = 1; j <= columns; j++) {
      // get the board square
      Coordinate c(i, j);
      BoardSquare& square = board.getSquare(c);
      // if there's no boat there, print a 'wave' or a 'splash'
      if (square.boatId() == -1) {
        if (square.torpedoed()) {
          cout << SPLASH;
        } else {
          cout << WAVE;
        }
      // if there is a boat there, print its formatted display ID
      } else {
        printBoatSquare(square.boatId(), square.torpedoed());
      }
    }
    // we've reached the end of the row, so print a new line
    cout << "\n";
  }
}

void BoardPrinter::printHeader(int columns) {
  // create a converter with the correct number of columns;
  // we will use this to convert column numbers into
  // Excel-style alphabetic strings
  CoordinateConverter converter(0, columns);
  // print the column headings row
  cout << "   |";
  for (int j = 1; j <= columns; j++) {
    // convert the column number to an Excel-style string
    string heading = converter.columnToString(j);
    cout << " " << heading;
    // if the heading is a single character, add a trailing space
    if (heading.length() < 2) {
      cout << " ";
    }
  }
  cout << "\n";
  // print the underline
  string underlineSegment = "___";
  cout << underlineSegment << "|";
  for (int j = 1; j <= columns; j++) {
    cout << underlineSegment;
  }
  cout << "\n";
}

void BoardPrinter::printBoatSquare(int boatId, bool isHit) {
  // boatId's are zero-based (for convenience accessing boats
  // from the fleet's array), but we want the display IDs to
  // start from 1
  string displayId = to_string(boatId + 1);
  // if the ID is only one digit, add a space after
  if (displayId.length() < 9) {
    displayId += " ";
  }
  // if the boat is hit, use white text on a red background
  if (isHit) {
    cout << HIT_BOAT_PREFIX;
  // otherwise, use black text on a grey ('bright black')
  // background
  } else {
    cout << BOAT_PREFIX;
  }
  cout << displayId << BOAT_SUFFIX;
}
