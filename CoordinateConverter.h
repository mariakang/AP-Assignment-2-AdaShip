#pragma once

#include <map>
#include <iterator>
#include <string>

using namespace std;

#include "Coordinate.h"

/** Converts between strings and Coordinate objects. */
class CoordinateConverter {
  private:
    /** The number of board rows. */
    int rows_;
    /** The number of board columns. */
    int columns_;
    /** A mapping from Excel-style column names to column numbers. */
    map<string, int> alphaLookup_;

    /** Populates the mapping from Excel-style column names to column numbers. */
    void initialiseLookup();

  public:
    /** Default constructor method. */
    CoordinateConverter() {}

    /** Constructor method which sets the number of rows and columns. */
    CoordinateConverter(int rows, int columns);

    /** Converts the given column number to its Excel-style representation. */
    string columnToString(int column);

    /** Returns a string representation of the given Coordinate. */
    string coordinateToString(Coordinate c);

    /** 
     * Returns a coordinate corresponding to the given string.
     *
     * If the string is not a valid coordinate, then (0, 0) is returned.
     */
    Coordinate getCoordinate(string s);

    /** Returns whether or not the given Coordinate is valid. */
    bool isValidCoordinate(Coordinate c);

    /** Returns whether or not the given string represents a valid Coordinate. */
    bool isValidCoordinateString(string s);
};