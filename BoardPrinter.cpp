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

void BoardPrinter::printBoardBoats(Player player) {
  // get player's board
  Board& board = player.board();
  // update the number of columns (used by 'printHeader')
  columns_ = board.columns();
  // print the header row
  printHeader();
  // iterate over the board
  int rows = board.rows();

}

void BoardPrinter::printHeader() {
  // create a converter with the correct number of columns;
  // we will use this to convert column numbers into
  // Excel-style alphabetic strings
  CoordinateConverter converter(0, columns_);
  // print the column headings row
  cout << "   |";
  for (int j = 1; j <= columns_; j++) {
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
  for (int j = 1; j <= columns_; j++) {
    cout << underlineSegment;
  }
}
