using namespace std;

#include "Boat.h"
#include "BoardSquare.h"
#include "Coordinate.h"
#include "Board.h"
#include "Fleet.h"

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

void Board::placeBoat(Coordinate c, int boatId) {
  for (int i = 1; i <= c.row(); i++) {
    Coordinate ci(i, c.column());
    BoardSquare& squarei = getSquare(ci);
    if (i < c.row()) {

    } else {
        squarei.setMaxBoatLengthVertical(0);
    }
  }

}
