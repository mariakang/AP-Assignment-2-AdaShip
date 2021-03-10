#pragma once

#include "BoardSquare.h"
#include "Coordinate.h"

#define MAX_ROWS 80
#define MAX_COLUMNS 80

/** Represents a player's board. */
class Board {
  private:
    /** The number of rows in the board. */
    int rows_;
    /** The number of columns in the board. */
    int columns_;
    /** A two-dimensional array of BoardSquare objects representing the board. */
    BoardSquare board_[MAX_ROWS][MAX_COLUMNS];
    /** The number of board squares which have not been torpedoed. */
    int remainingTargets_;

    /** Sets the maximum length of boat which could be placed in each square. */
    void initialiseSquares();

  public:
    /** Default constructor method (required by Player class). */
    Board() {
      rows_ = 0;
      columns_ = 0;
      remainingTargets_ = 0;
      initialiseSquares();
    }

    /** Constructor method which sets the numbers of rows and columns. */
    Board(int rows, int columns);

    /** Returns the number of rows the board has. */
    int rows() {
      return rows_;
    }

    /** Returns the number of columns the board has. */
    int columns() {
      return columns_;
    }

    /** Returns the BoardSquare at the given Coordinate. */
    BoardSquare& getSquare(Coordinate c) {
      return board_[c.row() - 1][c.column() - 1];
    }

    /** Returns the number of squares which have not been torpedoed. */
    int remainingTargets() {
      return remainingTargets_;
    }

    /** Reduces the number of remaining targets by 1. */
    void decrementRemainingTargets() {
      remainingTargets_--;
    }

    /** Resets each square's relative probability to zero. */
    void clearProbabilities();

    /** Assignment operator. */
    Board& operator=(const Board& rhs) {
      // if the rhs already equals this object, no need to do anything
      if (this != &rhs) {
        // copy over the attributes of rhs
        rows_ = rhs.rows_;
        columns_ = rhs.columns_;
        remainingTargets_ = rhs.remainingTargets_;
        for (int i = 0; i < rows_; i++) {
          for (int j = 0; j < columns_; j++) {
            board_[i][j] = rhs.board_[i][j];
          }
        }
      }
      // return the dereferenced pointer
      return (*this);
    }

};