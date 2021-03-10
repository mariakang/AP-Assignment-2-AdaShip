#include "Board.h"

/** Constructor method which sets the numbers of rows and columns. */
Board::Board(int rows, int columns) {
  // cap the number of rows at MAX_ROWS
  if (rows <= MAX_ROWS) {
    rows_ = rows;
  } else {
    rows_ = MAX_ROWS;
  }
  // cap the number of columns at MAX_COLUMNS
  if (columns <= MAX_COLUMNS) {
    columns_ = columns;
  } else {
    columns_ = MAX_COLUMNS;
  }
  // set the remaining targets to equal the number of squares on the board
  remainingTargets_ = rows_ * columns_;
  // initialise the squares, setting the maximum boat lengths
  initialiseSquares();
}

/** Sets the maximum length of boat which could be placed in each square. */
void Board::initialiseSquares() {
  // iterate over the rows
  for (int i = 0; i < rows_ && i < MAX_ROWS; i++) {
    // calculate the maximum length of boat which could be placed vertically
    int maxBoatLengthVertical = rows_ - i;
    // iterate over the columns
    for (int j = 0; j < columns_ && j < MAX_COLUMNS; j++) {
      // calculate the maximum length of boat which could be placed horizontally
      int maxBoatLengthHorizontal = columns_ - j;
      // update the board square with these calculated values
      board_[i][j].setMaxBoatLengthVertical(maxBoatLengthVertical);
      board_[i][j].setMaxBoatLengthHorizontal(maxBoatLengthHorizontal);
    }
  }
}

/** Resets each square's relative probability to zero. */
void Board::clearProbabilities() {
  // iterate over the board
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_ ; j++) {
      // set the square's relative probability to zero.
      board_[i][j].resetProbability();
    }
  }
}