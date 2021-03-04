using namespace std;

#include "Boat.h"
#include "BoardSquare.h"
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
