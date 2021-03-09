#pragma once

#include <map>
#include <iterator>
#include <string>

using namespace std;

#include "Coordinate.h"

class CoordinateConverter {
  private:
    int rows_;
    int columns_;
    map<string, int> alphaLookup_;
    void initialiseLookup();

  public:
    CoordinateConverter() {}

    CoordinateConverter(int rows, int columns);

    string columnToString(int column);

    string coordinateToString(Coordinate c);

    Coordinate getCoordinate(string s);

    bool isValidCoordinate(Coordinate c);

    bool isValidCoordinateString(string s);
};