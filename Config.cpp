#include <iostream>
#include <fstream>

using namespace std;

#include "Boat.h"
#include "Config.h"
#include "Fleet.h"

#define CONFIG_FILE_NAME "adaship_config.ini" // the name of the config file to be read

// headings which could appear in the config file
#define BOARD "board"
#define BOAT "boat"
#define MINES "mines"

/** 
 * Reads the config file and updates the Config object accordingly.
 *
 * Returns whether or not the action completed successfully.
 */
bool Config::readConfigFile() {
  cout << "Reading config file...\n";
  // define an iterator to iterate over the config file, and a variable to
  // store the current line's contents
  ifstream configFile;
  string configLine = "";

  // set variables to check whether the board dimensions have been set and
  // at least one boat has been added
  bool dimensionsSet = false;
  bool boatAdded = false;

  // open the config file
  configFile.open(CONFIG_FILE_NAME);

  // iterate over the file (line by line until the end of the file is reached)
  while(!configFile.eof()) {
    getline(configFile, configLine);
    int index = 0;
    // Store the string up to ':' (if no ':' is found, the line is ignored)
    string category = "";
    while (index < configLine.length() && configLine[index] != ':') {
      // convert to lowercase
      category += tolower(configLine[index]);
      index++;
    }
    // skip over the ':'
    index += 1;
    // check the category; if it's recognised, update the config; otherwise move on
    if (category == BOARD) {
      if (setBoardDimensions(configLine, index)) {
        dimensionsSet = true;
      }
    } else if (category == BOAT) {
      if (addBoat(configLine, index)) {
        boatAdded = true;
      }
    } else if (category == MINES) {
      setNumberOfMines(configLine, index);
    }
  }
  cout << "\tFleet size: " << fleet_.size() << "\n";
  // close the file
  configFile.close();
  cout << "\nFinished reading config file.\n\n";
	return (dimensionsSet && boatAdded);
}

/** 
 * Sets the board dimensions to those specified by the given
 * line of the config file, beginning at the given index.
 *
 * Returns whether or not the action was completed successfully.
 */
bool Config::setBoardDimensions(string configLine, int index) {
  string rows = "";
  string columns = "";
  // skip over the non-numeric characters
  while (index < configLine.length() && !isdigit(configLine[index])) {
    index++;
    // end of line reached => invalid config file line
    if (index == configLine.length()) {
      return false;
    }
  }
  // add each numeric character to the 'rows' string
  while (index < configLine.length() && isdigit(configLine[index])) {
    rows += configLine[index];
    index++;
    // end of line reached => invalid config file line
    if (index == configLine.length()) {
      return false;
    }
  }
  // skip over the non-numeric characters
  while (index < configLine.length() && !isdigit(configLine[index])) {
    index++;
    // end of line reached => invalid config file line
    if (index == configLine.length()) {
      return false;
    }
  }
  // add each numeric character to the 'columns' string
  while (index < configLine.length() && isdigit(configLine[index])) {
    columns += configLine[index];
    index++;
  }
  // convert strings to ints and update class members
  rows_ = stoi(rows);
  columns_ = stoi(columns);
  cout << "\tBoard size set to " << rows_ << " x " << columns_ << "\n";
  return true; 
}

/** 
 * Creates a Boat from the given line of the config file,
 * beginning at the given index, and adds it to the Fleet.
 *
 * Returns whether or not the action was completed successfully.
 */
bool Config::addBoat(string configLine, int index) {
  // if the first line is a space, skip over it
  if (configLine[index] == ' ') {
    index++;
  }
  // add each character up to the ',' to 'name'
  string name = "";
  while (index < configLine.length() && configLine[index] != ',') {
    name += configLine[index];
    index++;
    // end of line reached => invalid config file line
    if (index == configLine.length()) {
      return false;
    }
  }
  // skip over any non-numeric characters
  while (index < configLine.length() && !isdigit(configLine[index])) {
    index++;
    // end of line reached => invalid config file line
    if (index == configLine.length()) {
      return false;
    }
  }
  // add each numeric character to 'len'
  string len = "";
  while (index < configLine.length() && isdigit(configLine[index])) {
    len += configLine[index];
    index++;
  }
  // convert 'len' string to an int, and create a new Boat
  Boat boat(name, stoi(len));
  // add the boat to the Fleet
  fleet_.add(boat);
  cout << "\tAdded boat " << boat.toString() << "\n";
  return true;
}

/** 
 * Sets the number of mines to that specified by the given line
 * of the config file, beginning at the given index.
 *
 * Returns whether or not the action was completed successfully.
 */
bool Config::setNumberOfMines(string configLine, int index) {
  string mines = "";
  // skip over the non-numeric characters
  while (index < configLine.length() && !isdigit(configLine[index])) {
    index++;
    // end of line reached => invalid config file line
    if (index == configLine.length()) {
      return false;
    }
  }
  // add each numeric character to the 'mines' string
  while (index < configLine.length() && isdigit(configLine[index])) {
    mines += configLine[index];
    index++;
  }
  // convert string to int and update class member
  mines_ = stoi(mines);
  cout << "\tNumber of mines set to " << mines_ <<  "\n";
  return true; 
}
