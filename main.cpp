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
  if (!validConfig) {
    cout << "There seems to be a problem with the config file.\n";
    return 0;
  }

  GameController controller(config);
  controller.menu();
  return 0;
}