#pragma once

#include <iostream>
#include <string>

using namespace std;

#include "BoardSquare.h"
#include "Player.h"

class BoardPrinter {
  private:
    void printHeader(int columns);
    void printBoatSquare(BoardSquare square);
    string displayId(int boatId);
    void printBoatsKeyLine(Player player, int boatId, bool setupMode);
    
  public:
    BoardPrinter() {}

    void printBoard(Player player, bool setupMode = false);

    void printBoardOpponentView(Player player);

    void printBoatsKey(Player player, bool setupMode);
};