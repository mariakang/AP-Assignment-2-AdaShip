#include "Boat.h"
#include "BoardSquare.h"
#include "Coordinate.h"
#include "Board.h"
#include "Fleet.h"

Board::Board(int rows, int columns) {
  if (rows <= MAX_ROWS) {
    rows_ = rows;
  } else {
    rows_ = MAX_ROWS;
  }
  if (columns <= MAX_COLUMNS) {
    columns_ = columns;
  } else {
    columns_ = MAX_COLUMNS;
  }
  remainingTargets_ = rows_ * columns_;
  initialiseSquares();
}

void Board::initialiseSquares() {
  for (int i = 0; i < rows_ && i < MAX_ROWS; i++) {
    int maxBoatLengthVertical = rows_ - i;
    for (int j = 0; j < columns_ && j < MAX_COLUMNS; j++) {
      int maxBoatLengthHorizontal = columns_ - j;
      board_[i][j].setMaxBoatLengthVertical(maxBoatLengthVertical);
      board_[i][j].setMaxBoatLengthHorizontal(maxBoatLengthHorizontal);
    }
  }
}
