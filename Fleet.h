#pragma once

#include "Boat.h"

#define MAX_BOATS 80

/** A collection of Boat objects. */
class Fleet {
  private:
    /** An array of Boat objects. */
    Boat boats_[MAX_BOATS];
    /** The number of Boat objects in the collection. */
    int size_;

  public:
    /** Default constructor method. */
    Fleet() {
      size_ = 0;
    }

    /** Returns the number of boats in the fleet. */
    int size() {
      return size_;
    }

    /** Returns a copy of the fleet. */
    Fleet copy();
    
    /** Adds a Boat object to the fleet. */
    void add(Boat& boat);

    /** Returns the boat at the given index. */
    Boat& boatAtIndex(int index) {
      return boats_[index];
    }

    /** Assignment operator. */
    Fleet& operator=(const Fleet& rhs) {
      // if the rhs already equals this object, no need to do anything
      if (this != &rhs) {
        // copy over the attributes of rhs
        size_ = rhs.size_;
        for (int i = 0; i < size_; i++) {
            boats_[i] = rhs.boats_[i];
        }
      }
      // return the dereferenced pointer
      return (*this);
    }
};