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
  int rows = board.rows();
  int columns = board.columns();
  // print the header row
  CoordinateConverter converter(rows, columns);
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
  // iterate over the board
}
