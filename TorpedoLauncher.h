#pragma once

#include "Coordinate.h"
#include "Player.h"

/** Launches torpedoes at a player's board. */
class TorpedoLauncher {
  private:
    /** Returns a randomly generated number between 1 and 'upperBound' inclusive. */
    int randomNumber(int upperBound);

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
    void calculateProbabilities(Player& player);

  public:
    /** Default constructor method. */
    TorpedoLauncher() {}

    /** 
     * Fires a torpedo at the given Coordinate on the given player's board.
     *
     * If 'outputToConsole' is set to true, then the target and result of
     * the torpedo ('Hit', 'Miss', etc.) will be output to the console. This
     * is set to true by default, but may be set to false to run experiments.
     *
     * Returns whether or not the action completed successfully.
     */
    bool torpedo(Player& player, Coordinate c, bool outputToConsole = true);

    /** 
     * Fires a torpedo at a randomly generated Coordinate on the given player's board.
     *
     * If 'outputToConsole' is set to true, then the target and result of
     * the torpedo ('Hit', 'Miss', etc.) will be output to the console. This
     * is set to true by default, but may be set to false to run experiments.
     *
     * Returns whether or not the action completed successfully.
     */
    bool torpedoRandom(Player& player, bool outputToConsole = true);

    /** 
     * Fires a torpedo at a calculated Coordinate on the given player's board.
     *
     * The algorithm checks to see if there are any untried targets adjacent
     * to previous hits. If so, then the first of these is chosen; otherwise
     * the relative probabilities of each square containing a boat are
     * calculated, and one with the highest probability is chosen.
     *
     * If 'outputToConsole' is set to true, then the target and result of
     * the torpedo ('Hit', 'Miss', etc.) will be output to the console. This
     * is set to true by default, but may be set to false to run experiments.
     *
     * Returns whether or not the action completed successfully.
     */
    bool torpedoCalculated(Player& player, bool outputToConsole = true);
};