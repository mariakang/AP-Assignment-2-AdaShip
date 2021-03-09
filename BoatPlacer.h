#pragma once

#include "Coordinate.h"
#include "Player.h"

class BoatPlacer {
  private:
    int randomNumber(int upperBound);

    bool removeBoat(Player& player, Boat& boat);

    bool placeBoatRandom(Player& player, int boatId);

  public:
    BoatPlacer() {}

    bool placeBoat(Player& player, int boatId, Coordinate c, bool vertical);

    bool placeRemainingBoats(Player& player);
};