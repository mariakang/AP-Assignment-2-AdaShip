#pragma once

#include <string>

using namespace std;

#include "Fleet.h"

class Config {
  private:
    int rows_;
    int columns_;
    int mines_;
    Fleet fleet_;
    bool setBoardDimensions(string configLine, int index);
    bool addBoat(string configLine, int index);
    bool setNumberOfMines(string configLine, int index);

  public:
    Config() {
      rows_ = 0;
      columns_ = 0;
      mines_ = 0;
    }

    bool readConfigFile();
    
    int rows() {
      return rows_;
    }
    
    int columns() {
      return columns_;
    }

    int mines() {
      return mines_;
    }

    Fleet& fleet() {
      return fleet_;
    }
};