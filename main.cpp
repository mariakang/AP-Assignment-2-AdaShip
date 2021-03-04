#include <iostream>
#include <string>

using namespace std;

#include "GameController.h"
#include "Boat.h"
#include "Fleet.h"
#include "Board.h"
#include "Config.h"
#include "BoardSquare.h"

int main() {
  Config config;
  bool validConfig = config.readConfigFile();
  GameController controller(config);
  Player player("Player 1", false, config);
  Fleet& testFleet = player.fleet();
  Boat b("Test", 4);
  testFleet.add(b);
  Boat& testBoat = testFleet.boatAtIndex(5);
  Board& testBoard = player.board();
  
  int rows = testBoard.rows();
  int cols = testBoard.columns();
  Coordinate test1 = controller.converter().getCoordinate("ab12");
  Coordinate test2 = controller.converter().getCoordinate("ae12");
  int eq = test1.equals(test2);

  cout << rows << " x " << cols << "\n";
  cout << test1.row() << ", " << test1.column() << "\n";
  cout << eq << "\n";
  cout << "fleet size: " << testFleet.size() << "\n";
  cout << testBoat.length() << "\n";
  test1 = test2;
  cout << test1.row() << ", " << test1.column() << "\n";

  Coordinate c(2, 4);
  BoardSquare& testSquare = testBoard.getSquare(c);
  cout << testSquare.maxBoatLengthHorizontal() << "\n";
  testSquare.setMaxBoatLengthHorizontal(30);
  cout << testSquare.maxBoatLengthHorizontal() << "\n";
  cout << testBoard.getSquare(c).maxBoatLengthHorizontal() << "\n";

  cout << testBoard.remainingTargets() << "\n";
  controller.torpedoRandom(player);
  cout << testBoard.remainingTargets() << "\n";
}