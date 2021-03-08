#include "Boat.h"
#include "Fleet.h"

Fleet Fleet::copy() {
  Fleet copy;
  copy.size_ = size_;
  for (int i = 0; i < size_; i++) {
    copy.boats_[i] = boats_[i].copy();
  }
  return copy;
}

void Fleet::add(Boat& boat) {
  if (size_ < MAX_BOATS) {
    size_++;
    boats_[size_ - 1] = boat;
  }
}