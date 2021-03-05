#include <iostream>
#include <string>

using namespace std;

#include "GameController.h"
#include "Boat.h"
#include "Fleet.h"
#include "Board.h"
#include "Config.h"
#include "BoardSquare.h"
#include "BoardPrinter.h"

int main() {
  Config config;
  bool validConfig = config.readConfigFile();
  GameController controller(config);
  Player player("Player 1", false, config);

  BoardPrinter printer;
  printer.printBoard(player);
  
  controller.placeBoatRandom(player, 1);
  printer.printBoard(player);
  controller.placeRemainingBoats(player);
  printer.printBoard(player);

  for (int i = 0; i < 20; i++) {
    controller.torpedoRandom(player);
    printer.printBoard(player);
    printer.printBoardOpponentView(player);
  }

  Coordinate c33(3, 3);
  BoardSquare& square33 = player.board().getSquare(c33);
  cout << "(3, 3):\n";
  cout << "\tMax vertical: " << square33.maxBoatLengthVertical() << "\n";
  cout << "\tMax horizontal: " << square33.maxBoatLengthHorizontal() << "\n";

  Coordinate c88(8, 8);
  BoardSquare& square88 = player.board().getSquare(c88);
  cout << "(8, 8):\n";
  cout << "\tMax vertical: " << square88.maxBoatLengthVertical() << "\n";
  cout << "\tMax horizontal: " << square88.maxBoatLengthHorizontal() << "\n";

}