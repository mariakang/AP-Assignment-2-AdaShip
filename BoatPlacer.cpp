#include "Board.h"
#include "BoardSquare.h"
#include "Boat.h"
#include "BoatPlacer.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"
#include "Player.h"

/** Returns a randomly generated number between 0 and 'upperBound' inclusive. */
int BoatPlacer::randomNumber(int upperBound) {
  // return a random number between 0 and 'upperBound'
  return rand() % (upperBound + 1);
}

/** 
 * Places the boat with the given boatId on the given player's board, at the given
 * Coordinate and orientation.
 *
 * Returns whether or not the action was completed successfully.
 */
bool BoatPlacer::placeBoat(Player& player, int boatId, Coordinate c, bool vertical) {
  // get the player's board
  Board& board = player.board();
  // if the coordinate is invalid, return false
  CoordinateConverter converter(board.rows(), board.columns());
  if (!converter.isValidCoordinate(c)) {
    return false;
  }
  // get the square and boat relating to the parameters
  BoardSquare& square = board.getSquare(c);
  Boat& boat = player.getBoat(boatId);

  // if the boat is already placed, then it will need to be removed; this must
  // happen before checking if the coordinate is valid (as removing the boat 
  // could make a previously invalid square valid), but we need to take note of
  // where it was, in case it can't be placed at the coordinate provided
  Coordinate originalStart = boat.start();
  if (boat.isPlaced()) {
    removeBoat(player, boat);
  }

  // if the boat length exceeds the maximum that can be placed in the square
  // (for the given orientation), return false
  if ((vertical && square.maxBoatLengthVertical() < boat.length())
        || (!vertical && square.maxBoatLengthHorizontal() < boat.length())) {
    // if the boat had previously been placed, replace it 
    if (!originalStart.isOrigin()) {
      placeBoat(player, boatId, originalStart, boat.vertical());
    }
    return false;
  }

  boat.setStart(c);
  boat.setVertical(vertical);

  if (vertical) {
    // iterate over the column the boat is being placed in,
    // from the top of the board to the end of the boat
    for (int i = 1; i < c.row() + boat.length(); i++) {
      Coordinate ci(i, c.column());
      BoardSquare& squarei = board.getSquare(ci);
      // if we haven't reached the start of the boat, just
      // update the maxBoatLengthVertical (to be the minimum of
      // its existing value, and the number of spaces before
      // the start of the newly placed boat)
      if (i < c.row()) {
        int currentMaxV = squarei.maxBoatLengthVertical();
        int newMaxV = c.row() - i;
        if (newMaxV < currentMaxV) {
          squarei.setMaxBoatLengthVertical(newMaxV);
        }
      // if we've reached the start of the boat, update both
      // maxBoatLengthVertical and maxBoatLengthHorizontal to 0,
      // set the boatId, and update the maxBoatLengthHorizontal
      // values of squares to the left of the current one
      } else {
        squarei.setMaxBoatLengthVertical(0);
        squarei.setMaxBoatLengthHorizontal(0);
        squarei.setBoatId(boatId);
        // iterate over squares to the left of this one
        for (int j = 1; j < c.column(); j++) {
          Coordinate cij(i, j);
          BoardSquare& squareij = board.getSquare(cij);
          // update the maxBoatLengthHorizontal (to be the minimum
          // of its existing value, and the number of spaces
          // before 'squarei')
          int currentMaxH = squareij.maxBoatLengthHorizontal();
          int newMaxH = c.column() - j;
          if (newMaxH < currentMaxH) {
            squareij.setMaxBoatLengthHorizontal(newMaxH);
          }
        }
      }
    }
  } else {
    // the boat must be placed horizontally
    // iterate over the row the boat is being placed in,
    // from the left of the board to the end of the boat
    for (int j = 1; j < c.column() + boat.length(); j++) {
      Coordinate cj(c.row(), j);
      BoardSquare& squarej = board.getSquare(cj);
      // if we haven't reached the start of the boat, just
      // update the maxBoatLengthHorizontal (to be the minimum of
      // its existing value, and the number of spaces before
      // the start of the newly placed boat)
      if (j < c.column()) {
        int currentMaxH = squarej.maxBoatLengthHorizontal();
        int newMaxH = c.column() - j;
        if (newMaxH < currentMaxH) {
          squarej.setMaxBoatLengthHorizontal(newMaxH);
        }
      // if we've reached the start of the boat, update both
      // maxBoatLengthVertical and maxBoatLengthHorizontal to 0,
      // set the boatId, and update the maxBoatLengthVertical
      // values of squares above the current one
      } else {
        squarej.setMaxBoatLengthVertical(0);
        squarej.setMaxBoatLengthHorizontal(0);
        squarej.setBoatId(boatId);
        // iterate over squares above this one
        for (int i = 1; i < c.row(); i++) {
          Coordinate cij(i, j);
          BoardSquare& squareij = board.getSquare(cij);
          // update the maxBoatLengthVertical (to be the minimum
          // of its existing value, and the number of spaces
          // above 'squarej')
          int currentMaxV = squareij.maxBoatLengthVertical();
          int newMaxV = c.row() - i;
          if (newMaxV < currentMaxV) {
            squareij.setMaxBoatLengthVertical(newMaxV);
          }
        }
      }
    }
  }
  return true;
}

/** 
 * Removes the given boat from the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
bool BoatPlacer::removeBoat(Player& player, Boat& boat) {
  // if the boat hasn't been placed, return false.
  if (!boat.isPlaced()) {
    return false;
  }
  // store the boat's start
  Coordinate start = boat.start();
  // reset the boat's start to (0, 0) (which isn't on the board)
  Coordinate origin;
  boat.setStart(origin);
  // note: we don't care about resetting the orientation; it's only
  // needed after the boat has been placed (and it will automatically
  // be set again when the boat is replaced)

  // update the board
  Board& board = player.board();
  // if the boat is placed vertically, starting from the bottom
  // of the boat and working up through the column until another
  // boat is encountered, update the squares
  if (boat.vertical()) {
    // get the maxBoatLengthVertical for the square immediately
    // underneath the boat (or 0 if at the bottom of the board)
    int lastBoatRow = start.row() + boat.length() - 1;
    int maxLengthV = 0;
    if (lastBoatRow < board.rows()) {
      Coordinate c(lastBoatRow + 1, start.column());
      maxLengthV = board.getSquare(c).maxBoatLengthVertical();
    }
    // as we move up through the board, increment maxLengthV
    // until we hit another boat
    for (int i = lastBoatRow; i >= 1; i--) {
      maxLengthV++;
      Coordinate ci(i, start.column());
      BoardSquare& squarei = board.getSquare(ci);
      // if we've gone past the start of the boat and hit another
      // boat, exit
      if (i < start.row() && squarei.boatId() > -1) {
        break;
      }
      // update the maxBoatLengthVertical
      squarei.setMaxBoatLengthVertical(maxLengthV);
      // if we're currently inside the boat, update the square's
      // boatId to -1, and update the maxBoatLengthHorizontal
      // for this square and the squares to the left
      if (i >= start.row()) {
        squarei.setBoatId(-1);
        // update squares to the left; get maxBoatLengthHorizontal
        // for the square immediately right of the boat (or 0 if at
        // the end of the board)
        int maxLengthH = 0;
        if (ci.column() < board.columns()) {
          Coordinate ciR(i, ci.column() + 1);
          maxLengthH = board.getSquare(ciR).maxBoatLengthHorizontal();
        }
        // as we move left through the board, increment maxLengthH
        // until we hit another boat
        for (int j = ci.column(); j >= 1; j--) {
          maxLengthH++;
          Coordinate cij(i, j);
          BoardSquare& squareij = board.getSquare(cij);
          // if we've hit another boat, exit
          if (squareij.boatId() > -1) {
            break;
          }
          // update the maxBoatLengthHorizontal
          squareij.setMaxBoatLengthHorizontal(maxLengthH);
        }
      }
    }
  // if the boat is placed horizontally, starting from the end
  // of the boat and working back through the row until another
  // boat is encountered, update the squares
  } else {
    // get the maxBoatLengthHorizontal for the square immediately
    // to the right of the boat (or 0 if at the end of the board)
    int lastBoatCol = start.column() + boat.length() - 1;
    int maxLengthH = 0;
    if (lastBoatCol < board.columns()) {
      Coordinate c(start.row(), lastBoatCol + 1);
      maxLengthH = board.getSquare(c).maxBoatLengthHorizontal();
    }
    // as we move left through the board, increment maxLengthH
    // until we hit another boat
    for (int j = lastBoatCol; j >= 1; j--) {
      maxLengthH++;
      Coordinate cj(start.row(), j);
      BoardSquare& squarej = board.getSquare(cj);
      // if we've gone past the start, check if we've hit another
      // boat, and exit if we have
      if (j < start.column() && squarej.boatId() > -1) {
        break;
      }
      // update the maxBoatLengthHorizontal
      squarej.setMaxBoatLengthHorizontal(maxLengthH);
      // if we're currently inside the boat, update the square's
      // boatId to -1, and update the maxBoatLengthVertical
      // for this square and the squares above it
      if (j >= start.column()) {
        squarej.setBoatId(-1);
        // update squares above; get maxBoatLengthVertical for the
        // square immediately underneath the boat (or 0 if at the
        // bottom of the board)
        int maxLengthV = 0;
        if (cj.row() < board.rows()) {
          Coordinate cjU(cj.row() + 1, j);
          maxLengthV = board.getSquare(cjU).maxBoatLengthVertical();
        }
        // as we move up through the board, increment maxLengthV
        // until we hit another boat
        for (int i = cj.row(); i >= 1; i--) {
          maxLengthV++;
          Coordinate cij(i, j);
          BoardSquare& squareij = board.getSquare(cij);
          // if we've hit another boat, exit
          if (squareij.boatId() > -1) {
            break;
          }
          // update the maxBoatLengthVertical
          squareij.setMaxBoatLengthVertical(maxLengthV);
        }
      }
    }
  }
  return true;
}

/** 
 * Places the boat with the given boatId on the given player's board, at the given
 * Coordinate and orientation.
 *
 * Returns whether or not the action was completed successfully.
 */
bool BoatPlacer::placeBoatRandom(Player& player, int boatId) {
  // get a random integer between 0 and 1 to determine orientation
  int orientationPicker = randomNumber(1);
  // if 0, vertical; if 1, horizontal
  bool vertical = (orientationPicker == 0);

  // get the length of the boat to be placed
  int len = player.getBoat(boatId).length();
  // get the player's board
  Board& board = player.board();
  // create an array to store the possible starting coordinates
  // (CoordinateStack provides index access)
  CoordinateStack possibleCoords; 
  // iterate over the board squares
  for (int i = 1; i < board.rows(); i++) {
    for (int j = 1; j < board.columns(); j++) {
      Coordinate cij(i, j);
      BoardSquare& squareij = board.getSquare(cij);
      // if the orientation is vertical and maxBoatLengthVertical
      // is at least 'len', or the orientation is horizontal and
      // maxBoatLengthHorizontal is at least 'len', add 'cij' to
      // 'possibleCoords' and update the size
      if ((vertical && squareij.maxBoatLengthVertical() >= len)
          || (!vertical && squareij.maxBoatLengthHorizontal() >= len)) {
        possibleCoords.push(cij);
      }
    }
  }
  // randomly select a coordinate from the array
  int randomIndex = randomNumber(possibleCoords.size() - 1);
  Coordinate c = possibleCoords.coordAtIndex(randomIndex);

  // place the boat at this coordinate using the chosen orientation
  return placeBoat(player, boatId, c, vertical);
}

/** Randomly places any unplaced boats on the given player's board. */
bool BoatPlacer::placeRemainingBoats(Player& player) {
  // iterate over the player's fleet ('i' corresponds to the boatId)
  for (int i = 0; i < player.fleet().size(); i++) {
    // check to see if the boat has been placed
    Boat& boat = player.getBoat(i);
    // if it hasn't, then place it
    if (!boat.isPlaced()) {
      if (!placeBoatRandom(player, i)) {
        return false;
      }
    }
  }
  return true;
}
