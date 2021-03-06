#include "Boat.h"
#include "BoardSquare.h"
#include "Coordinate.h"
#include "Board.h"
#include "Fleet.h"

Board::Board(Config config) {
  if (config.rows() <= MAX_ROWS) {
    rows_ = config.rows();
  } else {
    rows_ = MAX_ROWS;
  }
  if (config.columns() <= MAX_COLUMNS) {
    columns_ = config.columns();
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
