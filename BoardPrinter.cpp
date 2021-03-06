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
//    33 -> yellow/orange
//    34 -> blue
//    35 -> magenta
//    37 -> white
//    41 -> red background
//    90 -> bright black
//    100 -> bright black background
//    104 -> bright blue background
 
#define WAVE "\033[34;104m ~ \033[0m"
#define SPLASH "\033[1;104m * \033[0m"
#define HIT "\033[1;31;100m x \033[0m"
#define MINE "\033[1;35;104m M \033[0m"
#define MINE_BOAT "\033[1;35;100m M \033[0m"
#define BOAT_PREFIX "\033[37;100m " // white text on grey ('bright black')
#define HIT_BOAT_PREFIX "\033[1;31;100m " // bold red text on grey ('bright black')
#define MINE_BOAT_PREFIX "\033[1;35;100m " // bold magenta text on grey ('bright black')
#define BOAT_SUFFIX "\033[0m" // resets the format
#define BOAT_KEY_PREFIX "\033[32m " // green text
#define HIT_BOAT_KEY_PREFIX "\033[33m " // yellow/orange text
#define SUNK_BOAT_KEY_PREFIX "\033[31m " // red text
#define PLACED_BOAT_PREFIX "\033[90m" // bright black text

// define the maximum number of columns the board can
// have before the boats key will be printed underneath
#define COLUMNS_THRESHOLD 30

/** Prints the given player's own board, displaying their own boats. */
void BoardPrinter::printBoard(Player player, bool setupMode) {
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
    cout << to_string(i) << " |";
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
      // if there is a boat there, print it
      } else {
        printBoatSquare(square.boatId(), square.torpedoed());
      }
    }
    // we've reached the end of the row; if the board is narrow enough,
    // and the row number corresponds to a boat display ID, print a line
    // of the boats key
    if (columns <= COLUMNS_THRESHOLD && i <= player.fleet().size()) {
      printBoatsKeyLine(player, i - 1, setupMode);
    } else {
      cout << "\n";
    }
  }
  cout << "\n";
  // the board is wide, print the boats key underneath it 
  if (columns > COLUMNS_THRESHOLD) {
    printBoatsKey(player, setupMode);
  }
}

/** Prints the opponent's view of the given player's board. */
void BoardPrinter::printBoardOpponentView(Player player) {
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
    cout << to_string(i) << " |";
    // iterate over the columns
    for (int j = 1; j <= columns; j++) {
      // get the board square
      Coordinate c(i, j);
      BoardSquare& square = board.getSquare(c);
      // if the square has been torpedoed, check for a boat
      if (square.torpedoed()) {
        // if there's no boat, print a 'splash' or a 'mine'
        if (square.boatId() == -1) {
          if (square.hasMine()) {
            cout << MINE;
          } else {
            cout << SPLASH;
          }
        // otherwise, print a 'hit' or a mine in a boat
        } else {
          if (square.hasMine()) {
            cout << MINE_BOAT;
          } else {
            cout << HIT;
          }
        }
      // if it hasn't been torpedoed, print a 'wave'
      } else {
        cout << WAVE;
      }
    }
    // we've reached the end of the row
    cout << "\n";
  }
  cout << "\n";
}

/** Prints the boats key. */
void BoardPrinter::printBoatsKey(Player player, bool setupMode) {
  // iterate over the player's fleet ('i' corresponds to the boatId)
  for (int i = 0; i < player.fleet().size(); i++) {
    printBoatsKeyLine(player, i, setupMode);
  }
  cout << "\n";
}

/** Prints a line of the boats key. */
void BoardPrinter::printBoatsKeyLine(Player player, int boatId, bool setupMode) {
  Boat& boat = player.getBoat(boatId);
  if (setupMode && boat.isPlaced()) {
    cout << PLACED_BOAT_PREFIX;
  }
  cout << "\t" << displayId(boatId) << " - ";
  if (!setupMode) {
    if (boat.damage() > 0) {
      if (boat.isSunk()) {
        cout << SUNK_BOAT_KEY_PREFIX;
      } else {
        cout << HIT_BOAT_KEY_PREFIX;
      }
    } else {
      cout << BOAT_KEY_PREFIX;
    }
  } else {
    cout << " ";
  }
  cout << boat.name() << BOAT_SUFFIX << "\n";
}

/** Prints the column headings and underline. */
void BoardPrinter::printHeader(int columns) {
  // create a converter with the correct number of columns;
  // we will use this to convert column numbers into
  // Excel-style alphabetic strings
  CoordinateConverter converter(0, columns);
  // print the column headings row
  cout << "\n   |";
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
  string underlineSegment = "---";
  cout << underlineSegment << "|";
  for (int j = 1; j <= columns; j++) {
    cout << underlineSegment;
  }
  cout << "\n";
}

/** Prints a board square occupied by the given boat. */
void BoardPrinter::printBoatSquare(int boatId, bool isHit) {
  // format the text, depending on whether the square is hit
  if (isHit) {
    cout << HIT_BOAT_PREFIX;
  } else {
    cout << BOAT_PREFIX;
  }
  cout << displayId(boatId) << BOAT_SUFFIX;
}

/** Returns a formatted string representing the given boatId. */
string BoardPrinter::displayId(int boatId) {
  // boatId's are zero-based (for convenience accessing boats
  // from the fleet's array), but we want the display IDs to
  // start from 1
  string displayId = to_string(boatId + 1);
  // if the ID is a single character, add a space after it
  if (displayId.length() < 2) {
    displayId += " ";
  }
  return displayId;
}
