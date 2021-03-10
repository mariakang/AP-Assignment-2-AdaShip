#pragma once

#include "Coordinate.h"
#include "Player.h"

/** Places boats on a player's board. */
class BoatPlacer {
  private:
    /** Returns a randomly generated number between 0 and 'upperBound' inclusive. */
    int randomNumber(int upperBound);

    /** 
     * Removes the given boat from the given player's board.
     *
     * Returns whether or not the action completed successfully.
     */
    bool removeBoat(Player& player, Boat& boat);

    /** 
     * Places the boat with the given boatId on the given player's board, at a
     * randomly chosen Coordinate and orientation.
     *
     * Returns whether or not the action was completed successfully.
     */
    bool placeBoatRandom(Player& player, int boatId);

  public:
    /** Constructor method. */
    BoatPlacer() {}

    /** 
     * Places the boat with the given boatId on the given player's board, at the given
     * Coordinate and orientation.
     *
     * Returns whether or not the action was completed successfully.
     */
    bool placeBoat(Player& player, int boatId, Coordinate c, bool vertical);

    /** 
     * Randomly places any unplaced boats on the given player's board.
     *
     * Returns whether or not the action was completed successfully.
     */
    bool placeRemainingBoats(Player& player);

    /** Resets the player's board. */
    void reset(Player& player);
};