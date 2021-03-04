#pragma once

using namespace std;

#include "Boat.h"

#define MAX_BOATS 80

class Fleet {
  private:
    Boat boats_[MAX_BOATS];
    int size_;

  public:
    Fleet() {
      size_ = 0;
    }

    int size() {
      return size_;
    }

    void setSize(int size) {
      size_ = size;
    }

    Fleet copy();
    
    void add(Boat& boat);

    Boat& boatAtIndex(int index) {
      return boats_[index];
    }

    void remove(int index);
    
    void removeBoat(Boat& boat);

    Fleet& operator=(const Fleet& rhs) {
      if (this != &rhs) {
        size_ = rhs.size_;
        for (int i = 0; i < size_; i++) {
            boats_[i] = rhs.boats_[i];
        }
      }
      return (*this);
    }
};