#pragma once

#include <map>
#include <iterator>
#include <string>

using namespace std;

#include "Board.h"
#include "Boat.h"
#include "Config.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"
#include "Player.h"

#define NUMBER_OF_PLAYERS 2

class GameController {
  private:
    int rows_;
    int columns_;
    CoordinateConverter converter_;

    int randomNumber(int upperBound);

  public:
    GameController(Config config);

    /** Returns whether or not the given Coordinate is valid. */
    bool isValidCoordinate(Coordinate coordinate) {
      int row = coordinate.row();
      int column = coordinate.column();
      return (row > 0 && row <= rows_ && column > 0 && column <= columns_);
    }

    CoordinateConverter converter() {
      return converter_;
    }

    bool torpedo(Player& player, Coordinate c);

    bool torpedoRandom(Player& player);

    bool placeBoat(Player& player, int boatId, Coordinate c, bool vertical);

    bool removeBoat(Player& player, Boat& boat);

    bool placeBoatRandom(Player& player, int boatId);

    bool placeRemainingBoats(Player& player);
};