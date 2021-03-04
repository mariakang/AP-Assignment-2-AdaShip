#pragma once

#include <iostream>
#include <string>

using namespace std;

#include "Player.h"

class BoardPrinter {
  private:
    int columns_;
    void printHeader();

  public:
    BoardPrinter() {
      columns_ = 0;
    }

    void printBoardBoats(Player player);
};