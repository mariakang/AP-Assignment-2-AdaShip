#include "Coordinate.h"
#include "CoordinateStack.h"

void CoordinateStack::push(Coordinate coordinate) {
  if (size_ < MAX_CAPACITY) {
    size_++;
    stack_[size_ - 1] = coordinate;
  }
}

Coordinate CoordinateStack::pop() {
  if (size_ > 0) {
    size_--;
  }
  return stack_[size_];
}
