#pragma once

#include <iostream>
#include <string>

using namespace std;

#include "Player.h"

class BoardPrinter {
  private:
    void printHeader(int columns);
    void printBoatSquare(int boatId, bool isHit);
    string displayId(int boatId);
    void printBoatsKeyLine(Player player, int boatId);
    void printBoatsKey(Player player);
    
  public:
    BoardPrinter() {}

    void printBoardBoats(Player player);
};