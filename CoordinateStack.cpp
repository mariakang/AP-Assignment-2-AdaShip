#include "Coordinate.h"
#include "CoordinateStack.h"

/** Adds a Coordinate object to the collection. */
void CoordinateStack::push(Coordinate coordinate) {
  // check that there is sufficient capacity to add another element
  if (size_ < MAX_CAPACITY) {
    // add the new coordinate to the end, and increment the size
    stack_[size_] = coordinate;
    size_++;
  }
}

/** Removes the most recently added coordinate from the collection and returns it. */
Coordinate CoordinateStack::pop() {
  // reduce the size by 1 (unless already empty)
  if (size_ > 0) {
    size_--;
  }
  // return the removed element (if the stack was empty to begin with, this will
  // return the last element to have been removed, but this will never happen in
  // practice, as the size will be checked before invoking this method)
  return stack_[size_];
}
