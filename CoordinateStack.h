#pragma once

#include "Coordinate.h"

#define MAX_CAPACITY 6400 // == MAX_ROWS * MAX_COLUMNS

/** A collection of easily accessible Coordinate objects. */
class CoordinateStack {
  private:
    /** An array of Coordinate objects. */
    Coordinate stack_[MAX_CAPACITY];
    /** The number of coordinates currently held. */
    int size_;

  public:
    /** Default constructor method. */
    CoordinateStack() {
      size_ = 0;
    }

    /** Returns the number of coordinates currently held. */
    int size() {
      return size_;
    }

    /** Sets the number of coordinates currently held. */
    void setSize(int size) {
      size_ = size;
    }
 
    /** Adds a Coordinate object to the collection. */
    void push(Coordinate coordinate);

    /** Removes the most recently added coordinate from the collection and returns it. */
    Coordinate pop();
  
    /** Returns the coordinate at the given index. */
    Coordinate coordAtIndex(int index) {
      return stack_[index];
    }

    /** Assignment operator. */
    CoordinateStack& operator=(const CoordinateStack& rhs) {
      // if the rhs already equals this object, no need to do anything
      if (this != &rhs) {
        // copy over the attributes of rhs
        size_ = rhs.size_;
        for (int i = 0; i < size_; i++) {
          stack_[i] = rhs.stack_[i];
        }
      }
      // return the dereferenced pointer
      return (*this);
    }
};