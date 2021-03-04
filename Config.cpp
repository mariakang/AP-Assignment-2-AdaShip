#include <iostream>
#include <fstream>

using namespace std;

#include "Boat.h"
#include "Config.h"
#include "Fleet.h"

#define CONFIG_FILE_NAME "adaship_config.ini" // the name of the config file to be read

/** Reads config file and populates Config. */
bool Config::readConfigFile() {
  // define an iterator to iterate over the config file, and a variable to
  // store the current line's contents
  ifstream configFile;
  string currentLineContents = "";

  // open the config file
  configFile.open(CONFIG_FILE_NAME);

  // store the first line's contents; this should include the board dimensions
  getline(configFile, currentLineContents);
  // extract the numbers of rows and columns; if none are found, the config file is invalid
  int index = 0;
  string rows = "";
  string columns = "";
  // skip over the non-numeric characters
  while (index < currentLineContents.length() && !isdigit(currentLineContents[index])) {
    index++;
    // end of line reached => invalid config file
    if (index == currentLineContents.length()) {
      return false;
    }
  }
  // add each numeric character to the 'rows' string
  while (index < currentLineContents.length() && isdigit(currentLineContents[index])) {
    rows += currentLineContents[index];
    index++;
    // end of line reached => invalid config file
    if (index == currentLineContents.length()) {
      return false;
    }
  }
  // skip over the non-numeric characters
  while (index < currentLineContents.length() && !isdigit(currentLineContents[index])) {
    index++;
    // end of line reached => invalid config file
    if (index == currentLineContents.length()) {
      return false;
    }
  }
  // add each numeric character to the 'columns' string
  while (index < currentLineContents.length() && isdigit(currentLineContents[index])) {
    columns += currentLineContents[index];
    index++;
  }
  // convert strings to ints and update class members
  rows_ = stoi(rows);
  columns_ = stoi(columns);
  cout << "Board size set to " << rows_ << " x " << columns_ << "\n";

  // iterate over the rest of the file (line by line until the end of the file is reached)
  while(!configFile.eof()) {
    getline(configFile, currentLineContents);
    // reset index to the start of the line
    index = 0;
    // each line should represent a boat; look for the ':'
    while (index < currentLineContents.length() && currentLineContents[index] != ':') {
      index++;
      // end of line reached => invalid config file
      if (index == currentLineContents.length()) {
        return false;
      }
    }
    // skip over the ':' and first space
    index += 2;
    // add each character up to the ',' to 'name'
    string name = "";
    while (index < currentLineContents.length() && currentLineContents[index] != ',') {
      name += currentLineContents[index];
      index++;
      // end of line reached => invalid config file
      if (index == currentLineContents.length()) {
        return false;
      }
    }
    // skip over the non-numeric characters
    while (index < currentLineContents.length() && !isdigit(currentLineContents[index])) {
      index++;
      // end of line reached => invalid config file
      if (index == currentLineContents.length()) {
        return false;
      }
    }
    // add each numeric character to 'len'
    string len = "";
    while (index < currentLineContents.length() && isdigit(currentLineContents[index])) {
      len += currentLineContents[index];
      index++;
    }
    // convert 'len' string to an int, and create a new Boat
    Boat boat(name, stoi(len));
    // add the boat to the Fleet
    fleet_.add(boat);
    cout << "Added boat " << boat.toString() << "\n";
  }
  cout << "Fleet size: " << fleet_.size() << "\n";
  // close the file
  configFile.close();
	return true;
}