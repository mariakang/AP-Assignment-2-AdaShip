#pragma once

#include <string>

using namespace std;

#include "Fleet.h"

class Config {
  private:
    /** The number of board rows. */
    int rows_;
    /** The number of board columns. */
    int columns_;
    /** The number of mines. */
    int mines_;
    /** A fleet of boats. */
    Fleet fleet_;

    /** 
     * Sets the board dimensions to those specified by the given
     * line of the config file, beginning at the given index.
     *
     * Returns whether or not the action was completed successfully.
     */
    bool setBoardDimensions(string configLine, int index);

    /** 
     * Creates a Boat from the given line of the config file,
     * beginning at the given index, and adds it to the Fleet.
     *
     * Returns whether or not the action was completed successfully.
     */
    bool addBoat(string configLine, int index);

    /** 
     * Sets the number of mines to that specified by the given line
     * of the config file, beginning at the given index.
     *
     * Returns whether or not the action was completed successfully.
     */
    bool setNumberOfMines(string configLine, int index);

  public:
    Config() {
      rows_ = 0;
      columns_ = 0;
      mines_ = 0;
    }

    /** 
     * Reads the config file and updates the Config object accordingly.
     *
     * Returns whether or not the action completed successfully.
     */
    bool readConfigFile();
    
    /** Returns the number of board rows specified by the config. */
    int rows() {
      return rows_;
    }
    
    /** Returns the number of board columns specified by the config. */
    int columns() {
      return columns_;
    }

    /** Returns the number of mines specified by the config. */
    int mines() {
      return mines_;
    }

    /** Returns the Fleet of boats specified by the config. */
    Fleet& fleet() {
      return fleet_;
    }
};