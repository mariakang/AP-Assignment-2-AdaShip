using namespace std;

#include "Boat.h"
#include "Fleet.h"

#define MAX_BOATS 80

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

void Fleet::remove(int index) {
  if (index >= 0 && index < size_) {
    size_--;
    for (int i = 0; i < size_; i++) {
      if (i >= index) {
        boats_[i] = boats_[i + 1];
      }
    }
  }
}

void Fleet::removeBoat(Boat& boat) {
  bool matchFound = false;
  for (int i = 0; i < size_; i++) {
    if (boats_[i].equals(boat)) {
      matchFound = true;
      size_--;
    }
    if (matchFound && i < size_) {
      boats_[i] = boats_[i + 1];
    }
  }
}
