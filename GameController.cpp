#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

#include "BoardPrinter.h"
#include "Config.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"
#include "CoordinateStack.h"
#include "GameController.h"
#include "Player.h"

#define MAX_LOOP_COUNT 1000
#define PAUSE_IN_SECONDS 1

/** Constructor function for GameController. */
GameController::GameController(Config config) {
  // set number of rows and columns
  rows_ = config.rows();
  columns_ = config.columns();
  if (config.mines() < rows_) {
    mines_ = config.mines();
  } else {
    mines_ = rows_;
  }
  showComputerBoard_ = true;
  CoordinateConverter converter(config);
  converter_ = converter;
  fleet_ = config.fleet().copy();

  // initialise random seed (required for the numbers to be generated randomly)
  // note: this should only be initialised once throughout the lifetime of the
  // program (which is why it is included here instead of inside the randomNumber
  // function)
  srand (time(NULL));
}

/** Returns a randomly generated number between 1 and 'upperBound' inclusive. */
int GameController::randomNumber(int upperBound) {
  // return a random number between 1 and 'upperBound'
  return rand() % upperBound + 1;
}

/** 
 * Fires a torpedo at the given Coordinate on the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
bool GameController::torpedo(Player& player, Coordinate c) {
  // coordinates supplied by users will have already been checked,
  // but if the torpedo request came from a mine, the coordinate
  // could lie outside the board boundaries
  if (!isValidCoordinate(c)) {
    return false;
  }
  // get the board and board square
  Board& board = player.board();
  BoardSquare& square = board.getSquare(c);
  // if the square has already been torpedoed, return false
  if (square.torpedoed()) {
    return false;
  }
  cout << converter_.coordinateToString(c) << "... ";
  // update the square to be torpedoed
  square.setTorpedoed(true);
  // this coordinate should be removed from the number of
  // potential targets (which is used to pick a random one)
  board.decrementRemainingTargets();
  // if the square doesn't contain a boat, it's a miss
  if (square.boatId() < 0) {
    cout << "Miss\n";
  // if it does contain a boat, get the boat and update its
  // damage 
  } else {
    cout << "Hit!\n";
    Boat& boat = player.getBoat(square.boatId());
    boat.hit();
    // if it's been sunk, update the number of surviving
    // boats the player has left
    if (boat.isSunk()) {
      player.decrementSurvivingBoats();
    }
    // add the four adjacent coordinates to the player's
    // target stack (to be used by their opponent's targeting
    // algorithm); ignore any coordinates which fall outside
    // the board, or have already been torpedoed
    if (c.row() > 1) {
      Coordinate above(c.row() - 1, c.column());
      if (!board.getSquare(above).torpedoed()) {
        player.addTarget(above);
      }
    }
    if (c.row() < board.rows()) {
      Coordinate below(c.row() + 1, c.column());
      if (!board.getSquare(below).torpedoed()) {
        player.addTarget(below);
      }
    }
    if (c.column() > 1) {
      Coordinate left(c.row(), c.column() - 1);
      if (!board.getSquare(left).torpedoed()) {
        player.addTarget(left);
      }
    }
    if (c.column() < board.columns()) {
      Coordinate right(c.row(), c.column() + 1);
      if (!board.getSquare(right).torpedoed()) {
        player.addTarget(right);
      }
    }
  }
  // if the square contains a mine, explode the surrounding
  // squares (if a square has already been torpedoed, nothing
  // will happen, so it doesn't matter that the loop includes
  // the current square again)
  if (square.hasMine()) {
    cout << "Mine!\n";
    for (int i = c.row() - 1; i <= c.row() + 1; i++) {
      for (int j = c.column() - 1; j <= c.column() + 1; j++) {
        Coordinate neighbour(i, j);
        torpedo(player, neighbour);
      }
    }
  }
  pause();
  return true;
}

/** 
 * Fires a torpedo at a randomly generated Coordinate on the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
bool GameController::torpedoRandom(Player& player) {
  Board& board = player.board();
  int target = randomNumber(board.remainingTargets());
  int counter = 0;
  for (int i = 1; i <= rows_; i++) {
    for (int j = 1; j <= columns_; j++) {
      Coordinate c(i, j);
      if (!board.getSquare(c).torpedoed()) {
        counter++;
        if (counter == target) {
          return torpedo(player, c);
        }
      }
    }
  }
  return false;
}

/** 
 * Calculates and updates the relative probabilities of each square of the
 * given player's board being occupied.
 *
 * These probabilities may be used by the player's opponent when invoking the
 * targeting algorithm.
 *
 * Note: This algorithm is 'fair' in the sense that it only uses information
 * which a human player would also have access to, if the roles were reversed.   
 */
void GameController::calculateProbabilities(Player& player) {
  // reset all probability densities to 0
  Board& board = player.board();
  board.clearProbabilities();

  // store the boat lengths in an array
  int fleetSize = player.fleet().size();
  int boatLengths[fleetSize];
  for (int i = 0; i < fleetSize; i++) {
    boatLengths[i] = player.getBoat(i).length();
  }
  // iterate over the board
  for (int i = 1; i <= board.rows(); i++) {
    for (int j = 1; j <= board.columns(); j++) {
      Coordinate cij(i, j);
      BoardSquare& squareij = board.getSquare(cij);
      // if the square has already been torpedoed, skip it (we don't
      // want to target it again)
      if (squareij.torpedoed()) {
        continue;
      }
      // iterate over the fleet
      for (int k = 0; k < fleetSize; k++) {
        int boatLength = boatLengths[k];
        // check to see if a boat of this length could be placed here
        // vertically; if it doesn't fit on the board, move on
        if (board.rows() - i + 1 >= boatLength) {
          bool canPlaceV = true;
          // moving downwards until the end of the boat, if we encounter
          // a 'miss', then we can't place the boat here
          for (int l = 1; l < boatLength; l++) {
            Coordinate cilj(i + l, j);
            BoardSquare& squareilj = board.getSquare(cilj);
            if (squareilj.isMiss()) {
              canPlaceV = false;
              break;
            }
          }
          // if we can place the boat here, increment the probabilities
          // of each occupied square
          if (canPlaceV) {
            for (int l = 0; l < boatLength; l++) {
              Coordinate cilj(i + l, j);
              board.getSquare(cilj).incrementProbability();
            }
          }
        }
        // check to see if a boat of this length could be placed here
        // horizontally; if it doesn't fit on the board, move on
        if (board.columns() - j + 1 >= boatLength) {
          bool canPlaceH = true;
          // moving right until the end of the boat, if we encounter
          // a 'miss', then we can't place the boat here
          for (int l = 1; l < boatLength; l++) {
            Coordinate cijl(i, j + l);
            BoardSquare& squareijl = board.getSquare(cijl);
            if (squareijl.isMiss()) {
              canPlaceH = false;
              break;
            }
          }
          // if we can place the boat here, increment the probabilities
          // of each occupied square
          if (canPlaceH) {
            for (int l = 0; l < boatLength; l++) {
              Coordinate cijl(i, j + l);
              board.getSquare(cijl).incrementProbability();
            }
          }
        }
      }
    }
  }
}

/** 
 * Fires a torpedo at a calculated Coordinate on the given player's board.
 *
 * The algorithm checks to see if there are any untried targets adjacent
 * to previous hits. If so, then the first of these is chosen; otherwise
 * the relative probabilities of each square containing a boat are
 * calculated, and one with the highest probability is chosen.
 *
 * Returns whether or not the action completed successfully.
 */
bool GameController::torpedoCalculated(Player& player) {
  // get the player's target stack
  CoordinateStack& targets = player.targets();
  // while the stack is non-empty, remove each target until a suitable candidate
  // has been found (at which point we can torpedo it and return true)
  while (targets.size() > 0) {
    Coordinate target = targets.pop();
    if (torpedo(player, target)) {
      return true;
    }
  }
  // if we've got to this point, it means the target stack is empty, so we need to
  // calculate the relative probabilities of each square
  calculateProbabilities(player);

  // iterate over the player's board, holding on to the highest probability, and
  // the first coordinate which has it
  Coordinate target;
  int highestProbability = 0;
  Board& board = player.board();
  for (int i = 1; i <= board.rows(); i++) {
    for (int j = 1; j <= board.columns(); j++) {
      Coordinate cij(i, j);
      int probability = board.getSquare(cij).probabilityDensity();
      if (probability > highestProbability) {
        highestProbability = probability;
        target = cij;
      }
    }
  }
  // fire a torpedo at the calculated target
  return torpedo(player, target); 
}

/** 
 * Places the boat with the given boatId on the given player's board, at the given
 * Coordinate and orientation.
 *
 * Returns whether or not the action was completed successfully.
 */
bool GameController::placeBoat(Player& player, int boatId, Coordinate c, bool vertical) {
  // if the coordinate is invalid, return false
  if (!isValidCoordinate(c)) {
    return false;
  }
  // get the board, square and boat relating to the parameters
  Board& board = player.board();
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
bool GameController::removeBoat(Player& player, Boat& boat) {
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
bool GameController::placeBoatRandom(Player& player, int boatId) {
  // get a random integer between 1 and 2 to determine orientation
  int orientationPicker = randomNumber(2);
  // if 1, vertical; if 2, horizontal
  bool vertical = (orientationPicker == 1);

  // get the length of the boat to be placed
  int len = player.getBoat(boatId).length();
  // get the player's board
  Board& board = player.board();
  // create an array to store the possible starting coordinates
  // (CoordinateStack provides index access)
  CoordinateStack possibleCoords; 
  // iterate over the board squares
  for (int i = 1; i < rows_; i++) {
    for (int j = 1; j < columns_; j++) {
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
  int randomIndex = randomNumber(possibleCoords.size()) - 1;
  Coordinate c = possibleCoords.coordAtIndex(randomIndex);

  // place the boat at this coordinate using the chosen orientation
  return placeBoat(player, boatId, c, vertical);
}

/** Randomly places any unplaced boats on the given player's board. */
bool GameController::placeRemainingBoats(Player& player) {
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

/** Randomly places mines in the given player's board. */
bool GameController::placeMines(Player& player) {
  int minesPlaced = 0;
  int loopCount = 0;
  // randomly place mines until they've all been placed, or
  // MAX_LOOP_COUNT is exceeded
  while (minesPlaced < mines_ && loopCount < MAX_LOOP_COUNT) {
    loopCount++;
    // generate a random coordinate (it doesn't matter if there's a boat)
    int randomRow = randomNumber(rows_);
    int randomColumn = randomNumber(columns_);
    Coordinate c(randomRow, randomColumn);
    // if there's no mine here already, place one (by setting hasMine to true)
    BoardSquare& square = player.board().getSquare(c);
    if (!square.hasMine()) {
      square.setHasMine(true);
      minesPlaced++;
    }
  }
  // return whether or not all mines have been successfully placed
  return minesPlaced == mines_;
}

bool GameController::gameSetup(Player& player,  bool minesMode) {
  BoardPrinter printer;

  if (player.isComputer()) {
    if (!placeRemainingBoats(player)) {
      cout << "Couldn't place Computer's boats.\n";
      return false;
    } else {
      if (showComputerBoard_) {
        printer.printBoard(player);
        pause();
      }
      cout << "Computer's boats have all been placed.\n";
      pause();
    }
  } else {
    int placedBoats = 0;
    int fleetSize = player.fleet().size();
    string input = "";
    while (placedBoats < fleetSize) {
      printer.printBoard(player, /* setupMode= */ true);
      cout << "Enter a boat number, reference and 'v/h' ";
      cout << "(e.g. '1 a1 v'), or press 'Enter' to auto-place your remaining boats: ";
      getline(cin, input);
      if (input.length() == 0) {
        if (!placeRemainingBoats(player)) {
          cout << "\nUnable to auto-place your remaining boats.\n";
          pause();
          return false;
        } else {
          placedBoats = fleetSize;
        }
      } else {
        string boatNumber = "";
        string reference = "";
        string vh = "";
        int index = 0;
        while (index < input.length() && isdigit(input[index])) {
          boatNumber += input[index];
          index++;
        }
        while (index < input.length() && !isalpha(input[index])) {
          index++;
        }
        while (index < input.length() && input[index] != ' ') {
          reference += input[index];
          index++;
        }
        while (index < input.length() && !isalpha(input[index])) {
          index++;
        }
        if (index < input.length()) {
          vh += input[index];
        }
        bool validInput = true;
        if (boatNumber.length() == 0 || stoi(boatNumber) > fleetSize || stoi(boatNumber) == 0) {
          cout << "You must enter a valid boat number.\n";
          validInput = false;
        }
        if (reference.length() == 0 || !isValidCoordinate(converter_.getCoordinate(reference))) {
          cout << "You must enter a valid grid reference.\n";
          validInput = false;
        }
        if (vh.length() != 1 || (tolower(vh[0]) != 'v' && tolower(vh[0]) != 'h')) {
          cout << "You must enter a valid orientation (v/h).\n";
          validInput = false;
        }
        if (validInput) {
          int boatId = stoi(boatNumber) - 1;
          Coordinate c = converter_.getCoordinate(reference);
          bool vertical = tolower(vh[0]) == 'v';
          if (!placeBoat(player, boatId, c, vertical)) {
            cout << "Unable to place boat " << boatNumber << " in " << reference << "\n.";
            pause(); 
          } else {
            placedBoats++;
          }
        } else {
          pause();
        }
      }
      if (placedBoats == fleetSize) {
        printer.printBoard(player);
        pause();
        cout << "\nAll of your boats have now been placed.\n";
        pause();
      }
    }
  }

  if (minesMode) {
    cout << "\nPlacing mines... ";
    if (placeMines(player)) {
      cout << mines_ << " mines have been placed.\n";
      pause();
      if (!player.isComputer() || showComputerBoard_) {
        printer.printBoard(player);
      }
    } else {
      cout << "Unable to place mines.\n";
    }
  }

  promptToContinue();
  return true;
}

/** 
 * Runs when the given player takes a turn against their opponent.
 *
 * When the turn is complete, if the player has won, the 'gameEnd' method will be
 * called. Otherwise, the user will be prompted to continue (or quit), and either
 * the program will be exited, or the next turn sequence automatically launched. 
 */
bool GameController::takeTurns(Player& player, Player& opponent, bool salvoMode) {
  cout << "\nIt's " << player.name() << "'s turn.\n";
  // if it's a two player game, the players may need to get into position, so prompt
  // to continue; otherwise just pause briefly.
  if (!player.isComputer() && !opponent.isComputer()) {
    promptToContinue();
  } else {
    pause();
  }
  // calculate the number of shots allowed
  int allowedShots = 1;
  if (salvoMode) {
    allowedShots = player.survivingBoats();
  }

  // check whether the player is the computer
  bool isComputer = player.isComputer();

  // print the player's board and target board (unless disabled)
  BoardPrinter printer;
  if (!isComputer || showComputerBoard_) {
    cout << "\nBoats:\n";
    printer.printBoard(player);
    pause();
    cout << "Targets:\n";
    printer.printBoardOpponentView(opponent);
    pause();  
  }

  // if the player is the computer, we will use autoFire mode, and
  // we will set validSelection to true, so that the user won't be
  // asked for input
  bool validSelection = isComputer;
  bool autoFire = isComputer;
  
  // initialise an array to store input
  string input = "";
  string targets[allowedShots];
  int targetsSize = 0;
  // prompt the user to enter target(s) until a valid selection has been made
  // (if fewer than the allowed number of targets are entered, fewer shots
  // will be taken)
  while (!validSelection) {
    if (allowedShots > 1) {
      cout << "\nEnter up to " << to_string(allowedShots) << " targets (or press 'Enter' to auto-fire): ";
    } else {
      cout << "\nEnter a target (or press 'Enter' to auto-fire): ";
    }
    getline(cin, input);
    // if the input string is empty, set autoFire and validSelection to true,
    // and exit the while loop
    if (input.length() == 0) {
      autoFire = true;
      validSelection = true;
      break;
    } else {
      // iterate over the input string, adding targets to the array
      int arrayIndex = 0;
      string token = "";
      for (int i = 0; i < input.length(); i++) {
        // if the character is a space, add the stored token to the array
        if (input[i] == ' ') {
          targets[arrayIndex] = token;
          // reset token
          token = "";
          // increment arrayIndex
          arrayIndex++;
          // if we've reached the number of required targets, ignore the
          // rest of the string
          if (arrayIndex >= allowedShots) {
            break;
          }
        // if the character isn't a space, add it to the token
        } else {
          token += input[i];
        }
      }
      // add the last token to the array
      if (arrayIndex < allowedShots) {
        targets[arrayIndex] = token;
        arrayIndex++;
      }
      // check for any invalid targets
      validSelection = true;
      for (int i = 0; i < allowedShots && i < arrayIndex; i++) {
        if (!isValidCoordinate(converter_.getCoordinate(targets[i]))) {
          cout << "'" << targets[i] << "' is not a valid target.\n";
          validSelection = false;
        }
      }
      // if all targets are valid, update targetsSize
      if (validSelection) {
        targetsSize = arrayIndex;
      }
    }
  }
  cout << "\n";
  // for each allowed shot, fire a torpedo at the opponent's
  // board; if fewer than the allowed number of targets were
  // specified, then only fire a torpedo at the specified
  // targets
  for (int i = 0; i < allowedShots; i++) {
    // make sure we have a target
    if (autoFire || i < targetsSize) {
      // if in autoFire mode, pick a target randomly
      if (autoFire) {
        torpedoRandom(opponent);
      // if not in autoFire mode, get the coordinate of the target
      } else {
        Coordinate c = converter_.getCoordinate(targets[i]);
        torpedo(opponent, c);
      }
      // print the target board
      printer.printBoardOpponentView(opponent);
      player.incrementShotsTaken();
      pause();
      // check to see if we have a winner
      if (opponent.survivingBoats() == 0) {
        // if so, run the gameEnd function
        return gameEnd(player);
      }
    }
  }
  // prompt the user to press enter to continue (or 'q' to quit)
  promptToContinue();
  // now it's the opponent's turn
  return takeTurns(opponent, player, salvoMode);
}

/** Runs when the given player has won. */
bool GameController::gameEnd(Player& player) {
  if (player.isComputer()) {
    cout << "\nOh no, you've lost! Better luck next time...\n\n";
  } else {
    cout << "\nCongratulations " << player.name();
    cout << "! You've won in " << to_string(player.shotsTaken()) << " shots!\n\n";
  }
  pause();
  promptToContinue();
  return true;
}

/** Prompts the user to press return before continuing. */
void GameController::promptToContinue() {
  string response = "";
  cout << "\nPress 'Enter' to continue... ('r' to return to the main menu or 'q' to quit): ";
  getline(cin, response);
  if (tolower(response[0]) == 'q') {
    quit();
  } else if (tolower(response[0]) == 'r') {
    menu();
  }
}

/** Quits the program. */
void GameController::quit() {
  // ask for confirmation
  string response = "";
  cout << "Are you sure? (Enter 'y' to confirm): ";
  getline(cin, response);
  if (tolower(response[0]) == 'y') {
    // if the response begins with 'y' (ignoring case), then exit the program
    cout << "\nGoodbye!\n";
    exit(EXIT_SUCCESS);
  }
}

/** Displays the main menu. */
void GameController::menu() {
  cout << "\nWelcome to AdaShip!\n\n";
  pause();
  while (true) {
    string input = "";
    cout << "Please select an option:\n";
    cout << "\t1 - One player v computer game\n";
    cout << "\t2 - Two player game\n";
    cout << "\t3 - One player v computer (salvo mode)\n";
    cout << "\t4 - Two player game (salvo mode)\n";
    cout << "\t5 - One player v computer (hidden mines)\n";
    cout << "\t6 - Two player game (hidden mines)\n";
    cout << "\t7 - Computer v computer (hidden mines)\n";
    cout << "\t8 - One player v computer (salvo mode with hidden mines)\n";
    cout << "\t9 - Two player game (salvo mode with hidden mines)\n";
    cout << "\tq - Quit\n\n";
    getline(cin, input);
    if (input.length() == 1) {
      char choice = input[0];
      switch (choice) {
        case '1': launchGame(/* players= */ 1, /* salvoMode= */ false, /* minesMode= */ false);
        case '2': launchGame(/* players= */ 2, /* salvoMode= */ false, /* minesMode= */ false);
        case '3': launchGame(/* players= */ 1, /* salvoMode= */ true, /* minesMode= */ false);
        case '4': launchGame(/* players= */ 2, /* salvoMode= */ true, /* minesMode= */ false);
        case '5': launchGame(/* players= */ 1, /* salvoMode= */ false, /* minesMode= */ true);
        case '6': launchGame(/* players= */ 2, /* salvoMode= */ false, /* minesMode= */ true);
        case '7': launchGame(/* players= */ 0, /* salvoMode= */ false, /* minesMode= */ true);
        case '8': launchGame(/* players= */ 1, /* salvoMode= */ true, /* minesMode= */ true);
        case '9': launchGame(/* players= */ 2, /* salvoMode= */ true, /* minesMode= */ true);
        case 'q': quit();
        default: cout << "Please enter a valid option.\n\n";
      }
    } else {
      cout << "Please enter a valid option.\n\n";
    }
  }
}

void GameController::launchGame(int numberOfHumanPlayers, bool salvoMode, bool minesMode) {
  Board board1(rows_, columns_);
  Player player1("Player 1", fleet_.copy(), board1);
  Board board2(rows_, columns_);
  Player player2("Player 2", fleet_.copy(), board2);

  if (numberOfHumanPlayers < 2) {
    if (numberOfHumanPlayers == 1) {
      player2.setName("Computer");
      player2.setIsComputer(true);
    } else if (numberOfHumanPlayers == 0) {
      player1.setName("Computer 1");
      player1.setIsComputer(true);
      player2.setName("Computer 2");
      player2.setIsComputer(true);
    }
    string input = "";
    cout << "\nWould you like to hide the computer's board? (Enter 'y' or the board will be shown): ";
    getline(cin, input);
    if (input.length() > 0 && tolower(input[0]) == 'y') {
      showComputerBoard_ = false;
    }
  }
  cout << "\nSetting up board for " << player1.name() << "...\n";
  pause();
  gameSetup(player1, minesMode);
  cout << "\nSetting up board for " << player2.name() << "...\n";
  pause();
  gameSetup(player2, minesMode);

  takeTurns(player1, player2, salvoMode);
}

/** Blocks further execution for PAUSE_IN_SECONDS seconds. */
void GameController::pause() {
  // convert seconds to milliseconds
  this_thread::sleep_for(chrono::milliseconds(PAUSE_IN_SECONDS * 1000));
}