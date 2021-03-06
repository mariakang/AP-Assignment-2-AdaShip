#include <map>
#include <iterator>
#include <string>

using namespace std;

#include "Coordinate.h"
#include "CoordinateConverter.h"

/** Constructor method which sets the number of rows and columns. */
CoordinateConverter::CoordinateConverter(int rows, int columns) {
  // set number of rows and columns
  rows_ = rows;
  columns_ = columns;
  // populate an 'alpha to column number' lookup map
  initialiseLookup();
}

/** Populates the mapping from Excel-style column names to column numbers. */
void CoordinateConverter::initialiseLookup() {
  // populate an 'alpha to column number' lookup map
  for (int i = 1; i <= columns_; i++) {
    alphaLookup_[columnToString(i)] = i;
  }
}

/** Converts the given column number to its Excel-style representation. */
string CoordinateConverter::columnToString(int column) {
  const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string alpha = "";
  // if the number is above 26, then an additional character (representing the
  // integer quotient when divided by 26) is prepended
  if (column > 26) {
    alpha += alphabet[(column - 1) / 26 - 1];
  }
  // the final character represents the remainder when divided by 26
  alpha += alphabet[(column - 1) % 26];
  return alpha;
}

/** Returns a string representation of the given Coordinate. */
string CoordinateConverter::coordinateToString(Coordinate c) {
  return columnToString(c.column()) + to_string(c.row());
}

/** 
 * Returns a coordinate corresponding to the given string.
 *
 * If the string is not a valid coordinate, then (0, 0) is returned.
 */
Coordinate CoordinateConverter::getCoordinate(string s) {
  // initialise an index to iterate over the string
  int index = 0;
  // initialise a default coordinate (i.e. (0, 0))
  Coordinate origin;
  // calculate the alphabetic portion of the string
  string alphabetic = "";
  while (index < s.length() && isalpha(s[index])) {
    alphabetic += toupper(s[index]);
    index++;
  }
  // if the alphabetic portion is not in the lookup map, return (0, 0)
  if (alphaLookup_.find(alphabetic) == alphaLookup_.end()) {
    return origin;
  }
  // otherwise, look up the column number it represents
  int column = alphaLookup_[alphabetic];

  // calculate the numeric part of the string
  string numeric = "";
  while (index < s.length() && isdigit(s[index])) {
    numeric += s[index];
    index++;
  }
  // if we haven't reached the end of the string, or the numeric part is
  // empty, return (0, 0)
  if (index < s.length() || numeric.length() == 0) {
    return origin;
  }
  // find the row corresponding to the numeric part of the string
  int row = stoi(numeric);

  // if the row number exceeds the total number of rows, return (0, 0)
  if (row > rows_) {
    return origin;
  }

  // if we've got to this point, it means the specified column and row
  // are valid, so return the corresponding coordinate
  Coordinate coordinate(row, column);
  return coordinate;
}

/** Returns whether or not the given Coordinate is valid. */
bool CoordinateConverter::isValidCoordinate(Coordinate c) {
  int row = c.row();
  int column = c.column();
  return (row > 0 && row <= rows_ && column > 0 && column <= columns_);
}

/** Returns whether or not the given string represents a valid Coordinate. */
bool CoordinateConverter::isValidCoordinateString(string s) {
  return isValidCoordinate(getCoordinate(s));
}
