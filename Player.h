#pragma once

#include <string>

using namespace std;

#include "Board.h"
#include "Boat.h"
#include "CoordinateStack.h"
#include "Fleet.h"

class Player {
  private:
    string name_;
    bool isComputer_;
    bool useTargetingAlgorithm_;
    int survivingBoats_;
    int shotsTaken_;
    Fleet fleet_;
    Board board_;
    CoordinateStack targets_;

  public:
    Player(string name, Fleet fleet, Board board) {
      name_ = name;
      isComputer_ = false;
      useTargetingAlgorithm_ = false;
      survivingBoats_ = fleet.size();
      shotsTaken_ = 0;
      fleet_ = fleet;
      board_ = board;
    }

    string name() {
      return name_;
    }

    void setName(string name) {
      name_ = name;
    }

    bool isComputer() {
      return isComputer_;
    }

    void setIsComputer(bool isComputer) {
      isComputer_ = isComputer;
    }

    bool useTargetingAlgorithm() {
      return useTargetingAlgorithm_;
    }

    void setUseTargetingAlgorithm(bool useTargetingAlgorithm) {
      useTargetingAlgorithm_ = useTargetingAlgorithm;
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

    CoordinateStack& targets() {
      return targets_;
    }

    Boat& getBoat(int boatId) {
      return fleet_.boatAtIndex(boatId);
    }

    void decrementSurvivingBoats() {
      survivingBoats_--;
    }

    void resetSurvivingBoats() {
      survivingBoats_ = fleet_.size();
    }

    void incrementShotsTaken() {
      shotsTaken_++;
    }

    void addTarget(Coordinate coordinate) {
      targets_.push(coordinate);
    }

    void clearTargets() {
      targets_.setSize(0);
    }
};