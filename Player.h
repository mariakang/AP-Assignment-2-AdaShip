#pragma once

#include <string>

using namespace std;

#include "Board.h"
#include "Boat.h"
#include "CoordinateStack.h"
#include "Fleet.h"

/** Represents a player. */
class Player {
  private:
    /** The player's name (e.g. 'Player 1', 'Computer'). */
    string name_;
    /** Whether or not the player is the computer. */
    bool isComputer_;
    /** Whether or not the player should use a targeting algorithm. */
    bool useTargetingAlgorithm_;
    /** The number of boats which have not yet been sunk. */
    int survivingBoats_;
    /** The number of torpedoes fired by the player. */
    int shotsTaken_;
    /** The player's collection of Boat objects. */
    Fleet fleet_;
    /** The board in which the player's boats are/will be placed. */
    Board board_;
    /** A collection of coordinates which have been deemed priority targets. */
    CoordinateStack targets_;

  public:
    /** Constructor method which sets the player's name, fleet and board. */
    Player(string name, Fleet fleet, Board board) {
      name_ = name;
      isComputer_ = false;
      useTargetingAlgorithm_ = false;
      survivingBoats_ = fleet.size();
      shotsTaken_ = 0;
      fleet_ = fleet;
      board_ = board;
    }

    /** Returns the player's name. */
    string name() {
      return name_;
    }

    /** Sets the player's name. */
    void setName(string name) {
      name_ = name;
    }

    /** Returns whether or not the player is the computer. */
    bool isComputer() {
      return isComputer_;
    }

    /** Sets whether or not the player is the computer. */
    void setIsComputer(bool isComputer) {
      isComputer_ = isComputer;
    }

    /** 
     * Returns whether or not the player will use a targeting algorithm
     * (as opposed to selecting targets randomly).
     */
    bool useTargetingAlgorithm() {
      return useTargetingAlgorithm_;
    }

    /** 
     * Sets whether or not the player will use a targeting algorithm (as
     * opposed to selecting targets randomly).
     */
    void setUseTargetingAlgorithm(bool useTargetingAlgorithm) {
      useTargetingAlgorithm_ = useTargetingAlgorithm;
    }

    /** Returns the number of boats which have not yet been sunk. */
    int survivingBoats() {
      return survivingBoats_;
    }

    /** Reduces the number of boats which have not been sunk by 1. */
    void decrementSurvivingBoats() {
      survivingBoats_--;
    }

    /** Returns the number of torpedoes fired by the player. */
    int shotsTaken() {
      return shotsTaken_;
    }

    /** Increases the number of torpedoes fired by 1. */
    void incrementShotsTaken() {
      shotsTaken_++;
    }

    /** Returns the player's fleet (i.e. collection of boats). */
    Fleet& fleet() {
      return fleet_;
    }

    /** Returns the boat at the given index in the player's fleet (0-based). */
    Boat& getBoat(int boatId) {
      return fleet_.boatAtIndex(boatId);
    }

    /** Returns the board on which the player's boats are/will be placed. */
    Board& board() {
      return board_;
    }

    /** 
     * Returns the collection of coordinates which have been deemed priority targets
     * (to be used by the player's opponent as part of the targeting algorithm).
     */
    CoordinateStack& targets() {
      return targets_;
    }

    /** 
     * Adds the given coordinate to the collection of priority targets (to be used by
     * the player's opponent as part of the targeting algorithm).
     */
    void addTarget(Coordinate coordinate) {
      targets_.push(coordinate);
    }
};
