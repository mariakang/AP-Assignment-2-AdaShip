#pragma once

#include <map>
#include <iterator>
#include <string>

using namespace std;

#include "Board.h"
#include "Boat.h"
#include "Config.h"
#include "Coordinate.h"
#include "Player.h"

class GameController {
  private:
    int rows_;
    int columns_;
    map<string, int> alphaLookup_;
    string columnToString(int column);
    string coordinateToString(Coordinate c);
    int randomNumber(int upperBound);

  public:
    GameController(Config config);

    bool isValidCoordinate(Coordinate coordinate) {
      int row = coordinate.row();
      int column = coordinate.column();
      return (row > 0 && row <= rows_ && column > 0 && column <= columns_);
    }

    Coordinate getCoordinate(string s);

    bool torpedo(Player& player, Coordinate c);

    bool torpedoRandom(Player& player);

};