#pragma once

#include<string>
#include <iostream>

using namespace std;

#include "Coordinate.h"

class Boat {
  private:
    string name_;
    int length_;
    Coordinate start_;
    bool vertical_;
    int damage_;
    void initialise() {
      Coordinate origin;
      start_ = origin;
      vertical_ = false;
      damage_ = 0;
    }

  public:
    Boat() {
      name_ = "";
      length_ = 0;
      initialise();
    }
    Boat(string name, int length) {
      name_ = name;
      length_ = length;
      initialise();
    }
    bool equals(Boat boat) {
      return boat.name() == name_ && boat.length() == length_ && boat.start().equals(start_);
    }
    Boat copy() {
      Boat copy;
      copy.name_ = name_;
      copy.length_ = length_;
      copy.start_ = start_;
      copy.vertical_ = vertical_;
      copy.damage_ = damage_;
      return copy;
    }
    string name() {
      return name_;
    }
    int length() {
      return length_;
    }
    Coordinate start() {
      return start_;
    }
    void setStart(Coordinate start) {
      start_ = start;
    }
    bool vertical() {
      return vertical_;
    }
    void setVertical(bool vertical) {
      vertical_ = vertical;
    }
    int damage() {
      return damage_;
    }
    bool isSunk() {
      return damage_ == length_;
    }
    void hit() {
      if (!isSunk()) {
        damage_++;
      }
    }
    char initial() {
      return toupper(name_[0]);
    }
    string toString() {
      string result = name_;
      result += " (length: ";
      result += to_string(length_);
      result += ")";
      return result;
    }
    Boat& operator=(const Boat& rhs) {
      if (this != &rhs) {
        name_ = rhs.name_;
        length_ = rhs.length_;
        start_ = rhs.start_;
        vertical_ = rhs.vertical_;
        damage_ = rhs.damage_;
      }
      return (*this);
    }
};