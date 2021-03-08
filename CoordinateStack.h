#pragma once

#include "Coordinate.h"

#define MAX_CAPACITY 6400 // == MAX_ROWS * MAX_COLUMNS

class CoordinateStack {
  private:
    Coordinate stack_[MAX_CAPACITY];
    int size_;

  public:
    CoordinateStack() {
      size_ = 0;
    }

    int size() {
      return size_;
    }

    void setSize(int size) {
      size_ = size;
    }

    void push(Coordinate coordinate);

    Coordinate pop();
  
    Coordinate coordAtIndex(int index) {
      return stack_[index];
    }

    CoordinateStack& operator=(const CoordinateStack& rhs) {
      if (this != &rhs) {
        size_ = rhs.size_;
        for (int i = 0; i < size_; i++) {
            stack_[i] = rhs.stack_[i];
        }
      }
      return (*this);
    }
};