#pragma once

#include <iostream>
#include <string>

using namespace std;

#include "Player.h"

class BoardPrinter {
  private:
    void printHeader(int columns);
    void printBoatSquare(int boatId, bool isHit);
    
  public:
    BoardPrinter() {}

    void printBoardBoats(Player player);
};