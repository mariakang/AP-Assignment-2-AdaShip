#pragma once

#include <iostream>
#include <string>

using namespace std;

#include "Player.h"

class BoardPrinter {
  public:
    BoardPrinter() {}

    void printBoardBoats(Player player);
};