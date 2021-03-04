#include <map>
#include <iterator>
#include <string>
#include <time.h>

using namespace std;

#include "Config.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"

/** Constructor function for CoordinateConverter (used by BoardPrinter). */
CoordinateConverter::CoordinateConverter(int rows, int columns) {
  // set number of rows and columns
  rows_ = rows;
  columns_ = rows;
  // populate an 'alpha to column number' lookup map
  initialiseLookup();
}

/** Constructor function for CoordinateConverter (used by GameController). */
CoordinateConverter::CoordinateConverter(Config config) {
  // set number of rows and columns
  rows_ = config.rows();
  columns_ = config.columns();
  // populate an 'alpha to column number' lookup map
  initialiseLookup();
}

void CoordinateConverter::initialiseLookup() {
  // populate an 'alpha to column number' lookup map
  for (int i = 1; i <= columns_; i++) {
    alphaLookup_[columnToString(i)] = i;
  }
}

/** Converts the given column number to its alphabetic representation. */
string CoordinateConverter::columnToString(int column) {
  const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string alpha = "";
  if (column > 26) {
    alpha += alphabet[(column - 1) / 26 - 1];
  }
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
  // initialise a default coordinate
  Coordinate coordinate;
  // calculate the alphabetic portion of the string
  string alphabetic = "";
  while (index < s.length() && isalpha(s[index])) {
    alphabetic += toupper(s[index]);
    index++;
  }
  // if the alphabetic portion is not in the lookup map, return (0, 0)
  if (alphaLookup_.find(alphabetic) == alphaLookup_.end()) {
    return coordinate;
  }
  // otherwise, look up the column number it represents
  int column = alphaLookup_[alphabetic];

  // calculate the numeric part of the string
  string numeric = "";
  while (index < s.length() && isdigit(s[index])) {
    numeric += s[index];
    index++;
  }
  // if we haven't reached the end of the string, return (0, 0)
  if (index < s.length()) {
    return coordinate;
  }
  // find the row corresponding to the numeric part of the string
  int row = stoi(numeric);

  // if the row number exceeds the total number of rows, return (0, 0)
  if (row > rows_) {
    return coordinate;
  }

  // update and return coordinate
  coordinate.setRow(row);
  coordinate.setColumn(column);
  return coordinate;
}