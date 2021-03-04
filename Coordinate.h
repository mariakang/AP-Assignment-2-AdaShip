#pragma once

using namespace std;

class Coordinate {
  private:
    int row_;
    int column_;
  public:
    Coordinate() {
      row_ = 0;
      column_ = 0;
    }
    Coordinate(int row, int column) {
      row_ = row;
      column_ = column;
    }
    int row() {
      return row_;
    }
    void setRow(int row) {
      row_ = row;
    }
    int column() {
      return column_;
    }
    void setColumn(int column) {
      column_ = column;
    }
    bool equals(Coordinate c) {
      return c.row() == row_ && c.column() == column_;
    }
    Coordinate& operator=(const Coordinate& rhs) {
      if (this != &rhs) {
        row_ = rhs.row_;
        column_ = rhs.column_;
      }
      return (*this);
    }
};