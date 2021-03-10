#include "Boat.h"
#include "Fleet.h"

/** Returns a copy of the fleet. */
Fleet Fleet::copy() {
  // instantiate a new Fleet object
  Fleet copy;
  // copy over the attributes
  copy.size_ = size_;
  for (int i = 0; i < size_; i++) {
    copy.boats_[i] = boats_[i].copy();
  }
  // return the new object
  return copy;
}

/** Adds a Boat object to the fleet. */
void Fleet::add(Boat& boat) {
  // check that we have capacity to add another boat
  if (size_ < MAX_BOATS) {
    // add the new boat to the end, and increment the size
    boats_[size_] = boat;
    size_++;
  }
}