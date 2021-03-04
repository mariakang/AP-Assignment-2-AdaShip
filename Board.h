#pragma once

#include <map>
#include <iterator>
#include <string>

using namespace std;

#include "Config.h"
#include "Coordinate.h"
#include "BoardSquare.h"

#define MAX_ROWS 80
#define MAX_COLUMNS 80

class Board {
  private:
    int rows_;
    int columns_;
    BoardSquare board_[MAX_ROWS][MAX_COLUMNS];
    int remainingTargets_;
    void initialiseSquares();

  public:
    Board() {
      rows_ = 0;
      columns_ = 0;
      remainingTargets_ = 0;
    }

    Board(Config config) {
      rows_ = config.rows();
      columns_ = config.columns();
      remainingTargets_ = rows_ * columns_;
      initialiseSquares();
    }

    int remainingTargets() {
      return remainingTargets_;
    }
    
    BoardSquare& getSquare(Coordinate c) {
      return board_[c.row() - 1][c.column() - 1];
    }

    void placeBoat(Coordinate c, int boatId);
  
    int rows() {
      return rows_;
    }
    int columns() {
      return columns_;
    }

    void decrementRemainingTargets() {
      remainingTargets_--;
    }

    Board& operator=(const Board& rhs) {
      if (this != &rhs) {
        rows_ = rhs.rows_;
        columns_ = rhs.columns_;
        remainingTargets_ = rhs.remainingTargets_;
        for (int i = 0; i < rows_; i++) {
          for (int j = 0; j < columns_; j++) {
            board_[i][j] = rhs.board_[i][j];
          }
        }
      }
      return (*this);
    }

};