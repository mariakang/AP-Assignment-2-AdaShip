#pragma once

#include "Coordinate.h"
#include "Player.h"

class TorpedoLauncher {
  private:
    int randomNumber(int upperBound);
    void calculateProbabilities(Player& player);

  public:
    TorpedoLauncher() {}

    bool torpedo(Player& player, Coordinate c, bool outputToConsole = true);

    bool torpedoRandom(Player& player, bool outputToConsole = true);

    bool torpedoCalculated(Player& player, bool outputToConsole = true);
};