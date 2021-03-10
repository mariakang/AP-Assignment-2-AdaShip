#include <iostream>

using namespace std;

#include "GameController.h"
#include "Config.h"

/** Driver function for 'AdaShip'. */
int main() {
  // instantiate a Config object
  Config config;
  // read in the relevant data from the config file ('adaship_config.ini')
  bool validConfig = config.readConfigFile();
  // if the config file was invalid in some way, report this and exit 
  if (!validConfig) {
    cout << "There seems to be a problem with the config file.\n";
    return 0;
  }

  // instantiate a GameController object, using the provided config
  GameController controller(config);
  // launch the starting menu; this presents the user with options, and executes
  // whichever they choose, looping until they quit the program
  controller.menu();
  return 0;
}