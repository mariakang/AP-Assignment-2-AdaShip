#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <time.h>

using namespace std;

#include "BoardPrinter.h"
#include "Config.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"
#include "GameController.h"
#include "Player.h"

/** Constructor function for GameController. */
GameController::GameController(Config config) {
  // set number of rows and columns
  rows_ = config.rows();
  columns_ = config.columns();
  showComputerBoard_ = true;
  CoordinateConverter converter(config);
  converter_ = converter;
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
  // damage; if it's been sunk, update the number of
  // surviving boats the player has left 
  } else {
    cout << "Hit!\n";
    Boat& boat = player.getBoat(square.boatId());
    boat.hit();
    if (boat.isSunk()) {
      player.decrementSurvivingBoats();
    }
  }
  // if the square contains a mine, explode the surrounding
  // squares (if a square has already been torpedoed, nothing
  // will happen, so it doesn't matter that the loop includes
  // current square again)
  if (square.hasMine()) {
    cout << "Mine!\n";
    for (int i = c.row() - 1; i <= c.row() + 1; i++) {
      for (int j = c.column() - 1; j <= c.column() + 1; j++) {
        Coordinate neighbour(i, j);
        torpedo(player, neighbour);
      }
    }
  }
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
  // if the boat length exceeds the maximum that can be placed in the square
  // (for the given orientation) return false
  if ((vertical && square.maxBoatLengthVertical() < boat.length())
        || (!vertical && square.maxBoatLengthHorizontal() < boat.length())) {
    return false;
  }
  // if the boat already has a start Coordinate (which isn't equal to (0, 0))
  // then the boat needs to be removed from the board first
  if (boat.isPlaced()) {
    removeBoat(player, boat);
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
  // create an array to store the possible starting coordinates,
  // keeping track of the number of items added
  Coordinate possibleCoords[rows_ * columns_]; 
  int possibleCoordsSize = 0;
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
        possibleCoords[possibleCoordsSize] = cij;
        possibleCoordsSize++;
      }
    }
  }
  // randomly select a coordinate from the array
  int randomIndex = randomNumber(possibleCoordsSize) - 1;
  Coordinate c = possibleCoords[randomIndex];

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

/** 
 * Runs when the given player takes a turn against their opponent.
 *
 * When the turn is complete, if the player has won, the 'gameEnd' method will be
 * called. Otherwise, the user will be prompted to continue (or quit), and either
 * the program will be exited, or the next turn sequence automatically launched. 
 */
bool GameController::takeTurn(Player& player, Player& opponent, bool salvoMode) {
  cout << "It's " << player.name() << "'s turn.\n";
  // calculate the number of shots allowed
  int allowedShots = 1;
  if (salvoMode) {
    allowedShots = player.survivingBoats();
  }

  bool isComputer = player.isComputer();
  // if the player is the computer, we don't need to ask for input
  bool validSelection = isComputer;
  bool autoFire = isComputer;
  
  // print the player's board and target board (unless disabled)
  BoardPrinter printer;
  if (!isComputer || showComputerBoard_) {
    printer.printBoard(player);
    printer.printBoardOpponentView(opponent);  
  }

  // initialise an array to store input
  string input = "";
  string targets[allowedShots];
  int targetsSize = 0;
  // prompt the user to enter target(s) until a valid selection has been made
  // (if fewer than the allowed number of targets are entered, fewer shots
  // will be taken)
  while (!validSelection) {
    if (allowedShots > 1) {
      cout << "Enter up to " << to_string(allowedShots) << " targets (or press 'Enter' to auto-fire): ";
    } else {
      cout << "Enter a target (or press 'Enter' to auto-fire): ";
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
      for (int i = 0; i < allowedShots; i++) {
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
  // for each allowed shot, fire a torpedo at the opponent's
  // board; if fewer than the allowed number of targets were
  // specified, then only fire a torpedo at the specified
  // targets
  for (int i = 0; i < allowedShots; i++) {
    // if in autoFire mode, pick a target randomly
    if (autoFire) {
      torpedoRandom(opponent);
      // print the target board
      printer.printBoardOpponentView(opponent);
      player.incrementShotsTaken();
    // if not in autoFire mode, check that we have a target
    } else {
      if (i < targetsSize) {
        Coordinate c = converter_.getCoordinate(targets[i]);
        torpedo(opponent, c);
        // print the target board
        printer.printBoardOpponentView(opponent);
        player.incrementShotsTaken();
      }
    }
    // check to see if we have a winner
    if (opponent.survivingBoats() == 0) {
      // if so, run the gameEnd function
      return gameEnd(player);
    }
  }
  // prompt the user to press enter to continue (or 'q' to quit)
  promptToContinue();
  // now it's the opponent's turn
  return takeTurn(opponent, player, salvoMode);
}

/** Runs when the given player has won. */
bool GameController::gameEnd(Player& player) {
  if (player.isComputer()) {
    cout << "Oh no - you've lost! Better luck next time...\n\n";
  } else {
    cout << "Congratulations" << player.name();
    cout << " - you've won in " << to_string(player.shotsTaken()) << " shots!\n\n";
  }
  promptToContinue();
  return true;
}

/** Prompts the user to press return before continuing. */
void GameController::promptToContinue() {
  string response = "";
  cout << "\nPress 'Enter' to continue... (or 'q' to quit)\n";
  getline(cin, response);
  if (tolower(response[0]) == 'q') {
    quit();
  }
}

/** Quits the program. */
void GameController::quit() {
  // ask for confirmation
  string response = "";
  cout << "Are you sure? (Enter 'y' to confirm.)\n" << endl;
  getline(cin, response);
  if (tolower(response[0]) == 'y') {
    // if the response begins with 'y' (ignoring case), then exit the program
    cout << "\nGoodbye!\n";
    exit(EXIT_SUCCESS);
  }
}

