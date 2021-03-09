#include <iostream>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

#include "Board.h"
#include "BoardSquare.h"
#include "Boat.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"
#include "CoordinateStack.h"
#include "Player.h"
#include "TorpedoLauncher.h"

/** Returns a randomly generated number between 1 and 'upperBound' inclusive. */
int TorpedoLauncher::randomNumber(int upperBound) {
  // return a random number between 1 and 'upperBound'
  return rand() % upperBound + 1;
}

/** 
 * Fires a torpedo at the given Coordinate on the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
bool TorpedoLauncher::torpedo(Player& player, Coordinate c, bool outputToConsole) {
  // coordinates supplied by users will have already been checked,
  // but if the torpedo request came from a mine, the coordinate
  // could lie outside the board boundaries
  Board& board = player.board();
  CoordinateConverter converter(board.rows(), board.columns());
  if (!converter.isValidCoordinate(c)) {
    return false;
  }
  // get the board square
  BoardSquare& square = board.getSquare(c);
  if (outputToConsole) {
    CoordinateConverter converter(board.rows(), board.columns());
    cout << converter.coordinateToString(c);
  }
  // if the square has already been torpedoed, return false
  if (square.torpedoed()) {
    if (outputToConsole) {
      cout << " has already been torpedoed.\n";
    }
    return false;
  }
  if (outputToConsole) {
    cout << "... ";
  }
  // update the square to be torpedoed
  square.setTorpedoed(true);
  // this coordinate should be removed from the number of
  // potential targets (which is used to pick a random one)
  board.decrementRemainingTargets();
  // if the square doesn't contain a boat, it's a miss
  if (square.boatId() < 0) {
    if (outputToConsole) {
      cout << "Miss\n";
    }
  // if it does contain a boat, get the boat and update its
  // damage 
  } else {
    if (outputToConsole) {
      cout << "Hit!\n";
    }
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
  // squares
  if (square.hasMine()) {
    if (outputToConsole) {
      cout << "Mine!\n";
    }
    for (int i = c.row() - 1; i <= c.row() + 1; i++) {
      for (int j = c.column() - 1; j <= c.column() + 1; j++) {
        if (i != c.row() || j != c.column()) {
          Coordinate neighbour(i, j);
          torpedo(player, neighbour);
        }
      }
    }
  }
  // if this isn't an experiment, pause for a second (we're doing this
  // inside this function as mine explosions will happen automatically)
  if (outputToConsole) {
    this_thread::sleep_for(chrono::milliseconds(1000));
  }
  return true;
}

/** 
 * Fires a torpedo at a randomly generated Coordinate on the given player's board.
 *
 * Returns whether or not the action completed successfully.
 */
bool TorpedoLauncher::torpedoRandom(Player& player, bool outputToConsole) {
  // get the player's board
  Board& board = player.board();
  // get a random number between 1 and the number of remaining targets
  int target = randomNumber(board.remainingTargets());
  // iterate over the board to find the 'target'th viable coordinate
  int counter = 0;
  for (int i = 1; i <= board.rows(); i++) {
    for (int j = 1; j <= board.columns(); j++) {
      Coordinate c(i, j);
      // if the square hasn't been torpedoed, increment the counter
      if (!board.getSquare(c).torpedoed()) {
        counter++;
        // if the counter has reached 'target', fire a torpedo at the
        // current coordinate
        if (counter == target) {
          return torpedo(player, c, outputToConsole);
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
void TorpedoLauncher::calculateProbabilities(Player& player) {
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
      // if the square has already been missed, skip it (we can't place any
      // boat here)
      if (!squareij.isMiss()) {
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
            // of each occupied square (unless already torpedoed)
            if (canPlaceV) {
              for (int l = 0; l < boatLength; l++) {
                Coordinate cilj(i + l, j);
                BoardSquare& squareilj = board.getSquare(cilj);
                if (!squareilj.torpedoed()) {
                  squareilj.incrementProbability();
                }
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
            // of each occupied square (unless already torpedoed)
            if (canPlaceH) {
              for (int l = 0; l < boatLength; l++) {
                Coordinate cijl(i, j + l);
                BoardSquare& squareijl = board.getSquare(cijl);
                if (!squareijl.torpedoed()) {
                  squareijl.incrementProbability();
                }
              }
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
bool TorpedoLauncher::torpedoCalculated(Player& player, bool outputToConsole) {
  // get the player's target stack
  CoordinateStack& targets = player.targets();
  // while the stack is non-empty, remove each target until a suitable candidate
  // has been found
  while (targets.size() > 0) {
    Coordinate target = targets.pop();
    // if the target hasn't already been torpedoed, torpedo it
    if (!player.board().getSquare(target).torpedoed()) {
      return torpedo(player, target, outputToConsole);
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
  return torpedo(player, target, outputToConsole); 
}