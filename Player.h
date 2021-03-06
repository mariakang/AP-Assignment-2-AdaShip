#pragma once

#include <string>

using namespace std;

#include "Board.h"
#include "Boat.h"
#include "Fleet.h"

class Player {
  private:
    string name_;
    bool isComputer_;
    int survivingBoats_;
    int shotsTaken_;
    Fleet fleet_;
    Board board_;

  public:
    Player(string name, bool isComputer, Config config) {
      name_ = name;
      isComputer_ = isComputer;
      survivingBoats_ = config.fleet().size();
      shotsTaken_ = 0;
      fleet_ = config.fleet().copy();
      Board board(config);
      board_ = board;
    }

    string name() {
      return name_;
    }

    bool isComputer() {
      return isComputer_;
    }

    int survivingBoats() {
      return survivingBoats_;
    }

    int shotsTaken() {
      return shotsTaken_;
    }

    Fleet& fleet() {
      return fleet_;
    }
    
    Board& board() {
      return board_;
    }

    Boat& getBoat(int boatId) {
      return fleet_.boatAtIndex(boatId);
    }

    void decrementSurvivingBoats() {
      survivingBoats_--;
    }

    void incrementShotsTaken() {
      shotsTaken_--;
    }
};