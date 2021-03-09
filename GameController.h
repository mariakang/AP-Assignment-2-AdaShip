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

class GameController {
  private:
    int rows_;
    int columns_;
    int mines_;
    bool showComputerBoard_;
    CoordinateConverter converter_;
    Fleet fleet_;

    int randomNumber(int upperBound);

  public:
    GameController(Config config);

    void menu();

    void launchGame(int numberOfHumanPlayers, bool salvoMode, bool minesMode);

    void launchExperiment();

    bool gameSetup(Player& player, bool minesMode);

    bool placeMines(Player& player);
    
    bool takeTurns(Player& player, Player& opponent, bool salvoMode);

    void postTorpedoRoutine(Player& player, Player& opponent);

    bool gameEnd(Player& player, Player& opponent);

    void pause();

    void promptToContinue();

    void quit();
};