#pragma once

#include<string>

using namespace std;

#include "Coordinate.h"

/** Contains information about a boat. */
class Boat {
  private:
    /** The boat's name. */
    string name_;
    /** The boat's length. */
    int length_;
    /** The boat's top left most coordinate. */
    Coordinate start_;
    /** Whether or not the boat is positioned vertically. */
    bool vertical_;
    /** The number of times the boat has been hit. */
    int damage_;

    /** Inititalises the boat to be unplaced and undamaged. */
    void initialise() {
      // set the boat's start to (0, 0), which is not on the board.
      Coordinate origin;
      start_ = origin;
      // set the orientation to horizontal (this is arbitrarily chosen)
      vertical_ = false;
      // set the damage to 0
      damage_ = 0;
    }

  public:
    /** Default constructor method (used by the Fleet class). */
    Boat() {
      name_ = "";
      length_ = 0;
      initialise();
    }

    /** Constructor method which sets the boat's name and length. */
    Boat(string name, int length) {
      name_ = name;
      length_ = length;
      initialise();
    }
    
    /** Returns a copy of the boat. */
    Boat copy() {
      Boat copy;
      copy.name_ = name_;
      copy.length_ = length_;
      copy.start_ = start_;
      copy.vertical_ = vertical_;
      copy.damage_ = damage_;
      return copy;
    }

    /** Returns the name of the boat. */
    string name() {
      return name_;
    }

    /** Returns the length of the boat. */
    int length() {
      return length_;
    }

    /** Returns the boat's top left most coordinate ((0, 0) by default). */
    Coordinate start() {
      return start_;
    }

    /** Sets the boat's top left most coordinate. */
    void setStart(Coordinate start) {
      start_ = start;
    }

    /** Returns whether or not the boat is positioned vertically (false by default). */
    bool vertical() {
      return vertical_;
    }

    /** Sets whether or not the boat is positioned vertically. */
    void setVertical(bool vertical) {
      vertical_ = vertical;
    }

    /** Returns the number of times the boat has been hit. */
    int damage() {
      return damage_;
    }

    /** Resets the number of times the boat has been hit to 0. */
    void resetDamage() {
      damage_ = 0;
    }

    /** Returns whether or not the boat has been placed on a board. */
    bool isPlaced() {
      return !start_.isOrigin();
    }

    /** Returns whether or not the boat has been sunk. */
    bool isSunk() {
      return damage_ == length_;
    }

    /** Updates the boat following a torpedo hit. */
    void hit() {
      if (!isSunk()) {
        damage_++;
      }
    }

    /** Returns a string containing the boat's name and length. */
    string toString() {
      return name_ + " (length: " + to_string(length_) + ")";
    }

    /** Assignment operator. */
    Boat& operator=(const Boat& rhs) {
      // if the rhs already equals this object, no need to do anything
      if (this != &rhs) {
        // copy over the attributes of rhs
        name_ = rhs.name_;
        length_ = rhs.length_;
        start_ = rhs.start_;
        vertical_ = rhs.vertical_;
        damage_ = rhs.damage_;
      }
      // return the dereferenced pointer
      return (*this);
    }
};