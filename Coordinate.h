#pragma once

/** Represents the location of a square on the board. */
class Coordinate {
  private:
    /** The board row (starting at 1). */
    int row_;
    /** The board column (starting at 1). */
    int column_;

  public:
    /** Default constructor method. */
    Coordinate() {
      row_ = 0;
      column_ = 0;
    }

    /** Constructor method which sets the row and column number. */
    Coordinate(int row, int column) {
      row_ = row;
      column_ = column;
    }

    /** Returns the board row (starting at 1). */
    int row() {
      return row_;
    }

    /** Returns the board column (starting at 1). */
    int column() {
      return column_;
    }

    /** Returns whether or not the coordinate equals (0, 0). */
    bool isOrigin() {
      return row_ == 0 && column_ == 0;
    }

    /** Assignment operator. */
    Coordinate& operator=(const Coordinate& rhs) {
      // if the rhs already equals this object, no need to do anything
      if (this != &rhs) {
        // copy over the attributes of rhs
        row_ = rhs.row_;
        column_ = rhs.column_;
      }
      // return the dereferenced pointer.
      return (*this);
    }
};