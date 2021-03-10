#include <iostream>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

#include "BoardPrinter.h"
#include "BoatPlacer.h"
#include "Config.h"
#include "Coordinate.h"
#include "CoordinateConverter.h"
#include "GameController.h"
#include "Player.h"
#include "TorpedoLauncher.h"

#define MAX_LOOP_COUNT 1000 // for the method that randomly places mines

/** Constructor method for GameController. */
GameController::GameController(Config config) {
  // set number of rows and columns
  rows_ = config.rows();
  columns_ = config.columns();
  if (config.mines() < rows_) {
    mines_ = config.mines();
  } else {
    mines_ = rows_;
  }
  showComputerBoard_ = true;
  gameInProgress_ = false;
  CoordinateConverter converter(rows_, columns_);
  converter_ = converter;
  fleet_ = config.fleet().copy();

  // initialise random seed (required for the numbers to be generated randomly)
  // note: this should only be initialised once throughout the lifetime of the
  // program (which is why it is included here instead of inside the classes
  // which generate random numbers)
  srand (time(NULL));
}

/** 
 * Displays the main menu and executes the chosen option, looping until the user
 * chooses to quit the program.
 */
void GameController::menu() {
  cout << "\nWelcome to AdaShip!\n";
  pause();
  // this will keep looping until the user chooses to exit the program
  while (true) {
    // display the options and capture the input entered
    string input = "";
    cout << "\nMenu:\n";
    cout << "\t1 - One player v computer game\n";
    cout << "\t2 - Two player game\n";
    cout << "\t3 - One player v computer (salvo mode)\n";
    cout << "\t4 - Two player game (salvo mode)\n";
    cout << "\t5 - One player v computer (hidden mines)\n";
    cout << "\t6 - Two player game (hidden mines)\n";
    cout << "\t7 - Computer v computer (hidden mines)\n";
    cout << "\t8 - One player v computer (salvo mode with hidden mines)\n";
    cout << "\t9 - Run experiment (random selection v targeting algorithm)\n";
    cout << "\tq - Quit\n";
    cout << "\nPlease select an option: ";
    getline(cin, input);
    // if a single character has been entered, convert it to char, and carry out the request
    if (input.length() == 1) {
      char choice = input[0];
      switch (choice) {
        case '1': launchGame(/* players= */ 1, /* salvoMode= */ false, /* minesMode= */ false); break;
        case '2': launchGame(/* players= */ 2, /* salvoMode= */ false, /* minesMode= */ false); break;
        case '3': launchGame(/* players= */ 1, /* salvoMode= */ true, /* minesMode= */ false); break;
        case '4': launchGame(/* players= */ 2, /* salvoMode= */ true, /* minesMode= */ false); break;
        case '5': launchGame(/* players= */ 1, /* salvoMode= */ false, /* minesMode= */ true); break;
        case '6': launchGame(/* players= */ 2, /* salvoMode= */ false, /* minesMode= */ true); break;
        case '7': launchGame(/* players= */ 0, /* salvoMode= */ false, /* minesMode= */ true); break;
        case '8': launchGame(/* players= */ 1, /* salvoMode= */ true, /* minesMode= */ true); break;
        case '9': launchExperiment(); break;
        case 'q': quit(); break;
        default: cout << "Please enter a valid option.\n\n";
      }
    // if more than one character was entered, it wasn't a valid option
    } else {
      cout << "Please enter a valid option.\n\n";
    }
  }
}

/** Launches a two player game with the given number of human players, and the given game modes. */
void GameController::launchGame(int numberOfHumanPlayers, bool salvoMode, bool minesMode) {
  // create two players
  Board board1(rows_, columns_);
  Player player1("Player 1", fleet_.copy(), board1);
  Board board2(rows_, columns_);
  Player player2("Player 2", fleet_.copy(), board2);

  // if at least one of the players is the computer, update the player(s)
  if (numberOfHumanPlayers < 2) {
    // if exactly one player is the computer, rename player2 'Computer' and set 'isComputer' to true
    if (numberOfHumanPlayers == 1) {
      player2.setName("Computer");
      player2.setIsComputer(true);
      // ask the user if they'd like to hide the computer's board (it will be shown by default)
      string input = "";
      cout << "\nWould you like to hide the computer's board? (Enter 'y' or the board will be shown): ";
      getline(cin, input);
      if (input.length() > 0 && tolower(input[0]) == 'y') {
        showComputerBoard_ = false;
      } else {
        showComputerBoard_ = true;
      }
      // ask the user if they'd like to increase the difficulty; increasing the difficulty will
      // make the computer use the targeting algorithm instead of picking randomly
      cout << "Would you like to increase the difficulty? (Enter 'y' to increase the difficulty): ";
      getline(cin, input);
      if (input.length() > 0 && tolower(input[0]) == 'y') {
        player2.setUseTargetingAlgorithm(true);
      }
    // if both players are computers, rename them to 'Computer 1' and 'Computer 2', and set isComputer
    // to true; both computers' boards will be shown
    } else if (numberOfHumanPlayers == 0) {
      player1.setName("Computer 1");
      player1.setIsComputer(true);
      player2.setName("Computer 2");
      player2.setIsComputer(true);
    }
  }
  // run the setup method for each player (which places ships and - if applicable - mines)
  if (numberOfHumanPlayers == 1) {
    cout << "\nSetting up your board...\n";
  } else {
    cout << "\nSetting up " << player1.name() << "'s board...\n";
  }
  pause();
  gameSetup(player1, minesMode);
  cout << "\nSetting up " << player2.name() << "'s board...\n";
  pause();
  gameSetup(player2, minesMode);

  // start a game
  gameInProgress_ = true;
  // keep track of whose turn it is (player1 will go first)
  bool player1sTurn = true;

  // keep taking turns (with player1 going first) until a winner is found
  while (gameInProgress_) {
    if (player1sTurn) {
      takeTurn(player1, player2, salvoMode);
    } else {
      takeTurn(player2, player1, salvoMode);
    }
    // switch players
    player1sTurn = !player1sTurn;
  }
}

/** Launches an experiment to compare random selection with the targeting algorithm. */
void GameController::launchExperiment() {
  // instantiate helper classes
  BoatPlacer placer;
  TorpedoLauncher launcher;
  // ask the user how many repetitions they'd like
  string input = "";
  bool validInput = false;
  // keep looping until the user has entered a valid number
  while(!validInput) {
    cout << "How many times would you like the experiment to be repeated? ";
    getline(cin, input);
    validInput = true;
    for (int i = 0; i < input.length(); i++) {
      if (!isdigit(input[i])) {
        validInput = false;
        break;
      }
    }
  }
  cout << "Running " << input << " tests...\n";
  int repetitions = stoi(input);
  // run an experiment for each repetition; using the same boat configuration,
  // count the number of random shots it takes to sink the fleet, followed by
  // the number of shots using the targeting algorithm
  int totalRandomShots = 0;
  int totalCalculatedShots = 0;
  for (int i = 1; i <= repetitions; i++) {
    cout << "\tTest " + to_string(i) << ": ";
    // create a player
    Board board(rows_, columns_);
    Player player("Player", fleet_.copy(), board);
    // randomly place the player's boats
    placer.placeRemainingBoats(player);
    // count the number of random shots it takes to sink this player's boats
    int randomShots = 0;
    while (player.survivingBoats() > 0) {
      randomShots++;
      launcher.torpedoRandom(player, /* outputToConsole= */ false);
    }
    // output the number of shots
    cout << "random: " << to_string(randomShots);
    // update the running total
    totalRandomShots += randomShots;
    // reset the player's surviving boats
    player.resetSurvivingBoats();
    // reset the player's board squares to not be torpedoed
    for (int j = 1; j <= player.board().rows(); j++) {
      for (int k = 1; k <= player.board().columns(); k++) {
        Coordinate cjk(j, k);
        player.board().getSquare(cjk).setTorpedoed(false);
      }
    }
    // reset the player's boat damage
    for (int j = 0; j < player.fleet().size(); j++) {
      player.getBoat(j).resetDamage();
    }
    // reset the player's target stack
    player.clearTargets();
    // count the number of targeted shots it takes to sink the player's boats
    int calculatedShots = 0;
    while (player.survivingBoats() > 0) {
      calculatedShots++;
      launcher.torpedoCalculated(player, /* outputToConsole= */ false);
    }
    // output the number of shots
    cout << ", calculated: " << to_string(calculatedShots) << "\n";
    // update the running total
    totalCalculatedShots += calculatedShots;
  }
  // calculate and output the average number of shots for each method
  // note: for simplicity, averages are rounded down to the nearest integer
  cout << "Average number of shots using random selection: " << to_string(totalRandomShots / repetitions) << "\n"; 
  cout << "Average number of shots using targeting algorithm: " << to_string(totalCalculatedShots / repetitions) << "\n\n";

  pause(); 
}

/** Returns a randomly generated number between 1 and 'upperBound' inclusive. */
int GameController::randomNumber(int upperBound) {
  // return a random number between 1 and 'upperBound'
  return rand() % upperBound + 1;
}

/** Randomly places mines in the given player's board. */
bool GameController::placeMines(Player& player) {
  int minesPlaced = 0;
  int loopCount = 0;
  // randomly place mines until they've all been placed, or
  // MAX_LOOP_COUNT is exceeded
  while (minesPlaced < mines_ && loopCount < MAX_LOOP_COUNT) {
    loopCount++;
    // generate a random coordinate (it doesn't matter if there's a boat)
    int randomRow = randomNumber(rows_);
    int randomColumn = randomNumber(columns_);
    Coordinate c(randomRow, randomColumn);
    // if there's no mine here already, place one (by setting hasMine to true)
    BoardSquare& square = player.board().getSquare(c);
    if (!square.hasMine()) {
      square.setHasMine(true);
      minesPlaced++;
    }
  }
  // return whether or not all mines have been successfully placed
  return minesPlaced == mines_;
}

/** Sets up the given player's board. */
bool GameController::gameSetup(Player& player,  bool minesMode) {
  // instantiate helper classes
  BoardPrinter printer;
  BoatPlacer placer;

  // if the player is a computer, automatically place all of its boats randomly
  if (player.isComputer()) {
    if (!placer.placeRemainingBoats(player)) {
      cout << "Couldn't place Computer's boats.\n";
      return false;
    } else {
      // if appropriate, show the player's board
      if (showComputerBoard_) {
        printer.printBoard(player);
        pause();
      }
      cout << "Computer's boats have all been placed.\n";
      pause();
    }
  // if the player is a human, ask them where they would like to place a boat
  } else {
    int placedBoats = 0;
    int fleetSize = player.fleet().size();
    // keep looping until all of the boats have been placed and confirmation is received
    string input = "";
    bool placementConfirmed = false;
    while (!placementConfirmed) {
      // print the player's board (setupMode => already placed boats will be greyed out)
      printer.printBoard(player, /* setupMode= */ true);
      cout << "Enter a boat number, reference and 'v/h' (e.g. '1 a1 v') to place a boat.\n";
      // if all boats have been placed, give the option to confirm placement or reset.
      if (placedBoats == fleetSize) {
        cout << "Alternatively, press 'Enter' to confirm placement, or 'r' to reset: ";
      // if not all boats have been placed, give the option to auto-place remaining boats or reset
      } else {
        cout << "Alternatively, press 'Enter' to auto-place your remaining boats, or 'r' to reset: ";
      }
      getline(cin, input);
      // if the user has pressed enter without specifying a position, either auto-place all of
      // their remaining boats or set confirmation received to true
      if (input.length() == 0) {
        // if all boats have been placed, set placementConfirmed to true, and exit the loop
        if (placedBoats == fleetSize) {
          placementConfirmed = true;
          break;
        // if not all boats have been placed, auto-place the remainder
        } else {
          if (!placer.placeRemainingBoats(player)) {
            cout << "\nUnable to auto-place your remaining boats.\n";
            pause();
            return false;
          // if all of the boats were successfully placed, update 'placedBoats'
          } else {
            placedBoats = fleetSize;
          }
        }
      // if the user entered a string beginning with 'r' (or 'R'), reset the board
      // and reset placedBoats to 0
      } else if (tolower(input[0]) == 'r') {
        placer.reset(player);
        placedBoats = 0;
      // if the user entered something else, check to see if it is valid input
      } else {
        string boatNumber = "";
        string reference = "";
        string vh = "";
        // iterate over the input string, storing each character in the appropriate variable
        int index = 0;
        // add any digit characters to the 'boatNumber' string
        while (index < input.length() && isdigit(input[index])) {
          boatNumber += input[index];
          index++;
        }
        // skip over non-alphabetic characters
        while (index < input.length() && !isalpha(input[index])) {
          index++;
        }
        // add the next characters up to the space to the 'reference' string (we'll check
        // validity later)
        while (index < input.length() && input[index] != ' ') {
          reference += input[index];
          index++;
        }
        // skip over the space and any non-alphabetic characters
        while (index < input.length() && !isalpha(input[index])) {
          index++;
        }
        // add the next alphabetic character to the 'vh' string (any substring beginning
        // with 'v' or 'h' will be treated as vertical or horizontal respectively)
        if (index < input.length()) {
          vh += input[index];
        }
        // assume the input is valid, and then verify each section
        bool validInput = true;
        // the boat number must exist, and be between 1 and the fleet size
        if (boatNumber.length() == 0 || stoi(boatNumber) > fleetSize || stoi(boatNumber) == 0) {
          cout << "You must enter a valid boat number.\n";
          validInput = false;
        }
        // the coordinate must lie inside the grid
        if (reference.length() == 0 || !converter_.isValidCoordinateString(reference)) {
          cout << "You must enter a valid grid reference.\n";
          validInput = false;
        }
        // the 'vh' string must be 'v' or 'h'
        if (vh.length() != 1 || (tolower(vh[0]) != 'v' && tolower(vh[0]) != 'h')) {
          cout << "You must enter a valid orientation (v/h).\n";
          validInput = false;
        }
        // if the input is valid, try to place the boat
        if (validInput) {
          // get the arguments to pass to '#placeBoat()'
          int boatId = stoi(boatNumber) - 1;
          bool alreadyPlaced = player.getBoat(boatId).isPlaced();
          Coordinate c = converter_.getCoordinate(reference);
          bool vertical = tolower(vh[0]) == 'v';
          // if the boat can't be placed here, loop again
          if (!placer.placeBoat(player, boatId, c, vertical)) {
            cout << "Unable to place boat " << boatNumber << " in " << reference << "\n.";
            pause();
          // if the boat's been successfully placed, and wasn't already placed, update 'placedBoats'
          } else {
            if (!alreadyPlaced) {
              placedBoats++;
            }
          }
        // if the input is invalid, loop again
        } else {
          pause();
        }
      }
      // if all boats have now been placed, output a message.
      if (placedBoats == fleetSize) {
        cout << "\nAll of your boats have now been placed.\n";
        pause();
      }
    }
  }

  // if in 'hidden mines' mode, randomly place the required number of mines
  if (minesMode) {
    cout << "\nPlacing mines... ";
    if (placeMines(player)) {
      cout << mines_ << " mines have been placed.\n";
      pause();
      // print the player's board if appropriate
      if (!player.isComputer() || showComputerBoard_) {
        printer.printBoard(player);
      }
    } else {
      cout << "Unable to place mines.\n";
    }
  }

  // prompt the user to continue
  promptToContinue();
  return true;
}

/** 
 * Runs when the given player takes a turn against their opponent.
 *
 * When the turn is complete, if the player has won, the 'gameEnd' method will be
 * called.
 */
bool GameController::takeTurn(Player& player, Player& opponent, bool salvoMode) {
  if (!player.isComputer() && opponent.isComputer()) {
    cout << "\nIt's your turn.\n";
  } else {
    cout << "\nIt's " << player.name() << "'s turn.\n";
  }
  // if it's a two player game, the players may need to get into position, so prompt
  // to continue; otherwise just pause briefly.
  if (!player.isComputer() && !opponent.isComputer()) {
    promptToContinue();
  } else {
    pause();
  }
  // calculate the number of shots allowed; if in salvo mode, this is the number of
  // surviving boats, otherwise it's 1
  int allowedShots = 1;
  if (salvoMode) {
    allowedShots = player.survivingBoats();
  }
  
  // instantiate helper classes
  BoardPrinter printer;
  TorpedoLauncher launcher;

  // check whether the player is the computer; if so, don't ask for user input
  if (player.isComputer()) {
    cout << "\n";
    // print the computer's boat and target boards, unless they should be hidden
    if (showComputerBoard_) {
      cout << "Boats:\n";
      printer.printBoard(player);
      pause();
      cout << "Targets:\n";
      printer.printBoardOpponentView(opponent);
      pause();  
    }
    // fire the given number of torpedoes
    for (int i = 0; i < allowedShots; i++) {
      // if the difficulty level was increased, use the targeting algorithm
      if (player.useTargetingAlgorithm()) {
        launcher.torpedoCalculated(opponent);
      // otherwise, pick a target randomly
      } else {
        launcher.torpedoRandom(opponent);
      }
      // print
      postTorpedoRoutine(player, opponent);
      // check to see if we have a winner
      if (opponent.survivingBoats() == 0) {
        // if so, run the gameEnd function
        return gameEnd(player, opponent);
      }
    }

  // if the player is human, print their boards and ask for target(s)
  } else {
    cout << "\nBoats:\n";
    printer.printBoard(player);
    pause();
    cout << "Targets:\n";
    printer.printBoardOpponentView(opponent);
    pause(); 

    // keep asking for targets until a valid selection has been made 
    bool validSelection = false;
    // initialise an array to store input
    string input = "";
    string targets[allowedShots];
    // prompt the user to enter target(s) until a valid selection has been made
    // (if fewer than the allowed number of targets are entered, fewer shots
    // will be taken)
    while (!validSelection) {
      if (allowedShots > 1) {
        cout << "\nEnter up to " << to_string(allowedShots) << " targets (or press 'Enter' to auto-fire): ";
      } else {
        cout << "\nEnter a target (or press 'Enter' to auto-fire): ";
      }
      getline(cin, input);
      // if the input string is empty, set validSelection to true, fire randomly,
      // and exit the while loop
      if (input.length() == 0) {
        validSelection = true;
        cout << "\n";
        for (int i = 0; i < allowedShots; i++) {
          launcher.torpedoRandom(opponent);
          postTorpedoRoutine(player, opponent);
          // check to see if we have a winner
          if (opponent.survivingBoats() == 0) {
          // if so, run the gameEnd function
            return gameEnd(player, opponent);
          }
        }
        break;
      } else {
        // iterate over the input string, adding targets to the array
        int arrayIndex = 0;
        string token = "";
        for (int i = 0; i < input.length(); i++) {
          // if the character is a space, add the stored token to the array
          if (input[i] == ' ') {
            targets[arrayIndex] = token;
            // reset token
            token = "";
            // increment arrayIndex
            arrayIndex++;
            // if we've reached the number of required targets, ignore the
            // rest of the string
            if (arrayIndex >= allowedShots) {
              break;
            }
          // if the character isn't a space, add it to the token
          } else {
            token += input[i];
          }
        }
        // add the last token to the array
        if (arrayIndex < allowedShots) {
          targets[arrayIndex] = token;
          arrayIndex++;
        }
        // check for any invalid targets (already torpedoed targets still count)
        validSelection = true;
        for (int i = 0; i < allowedShots && i < arrayIndex; i++) {
          if (!converter_.isValidCoordinateString(targets[i])) {
            cout << "'" << targets[i] << "' is not a valid target.\n";
            validSelection = false;
          }
        }
        // if all targets are valid, fire a torpedo at each one
        if (validSelection) {
          cout << "\n";
          for (int i = 0; i < arrayIndex; i++) {
            launcher.torpedo(opponent, converter_.getCoordinate(targets[i]));
            postTorpedoRoutine(player, opponent);
            // check to see if we have a winner
            if (opponent.survivingBoats() == 0) {
            // if so, run the gameEnd function
              return gameEnd(player, opponent);
            }
          }
        }
      }
    }
  }
  // prompt the user to press enter to continue (or 'q' to quit)
  promptToContinue();
  // now it's the opponent's turn
  return true;
}

/** 
 * Increments the number of shots taken by the given player, and prints their target
 * board to the console.
 *
 * Runs immediately after 'player' has fired a torpedo at 'opponent'.
 */
void GameController::postTorpedoRoutine(Player& player, Player& opponent) {
  // update the number of shots taken
  player.incrementShotsTaken();
  // print the player's target board, and pause briefly
  if (!player.isComputer() || showComputerBoard_) {
    BoardPrinter printer;
    printer.printBoardOpponentView(opponent);
    pause();
  }
}

/** Runs when the given player has beaten their opponent. */
bool GameController::gameEnd(Player& player, Player& opponent) {
  // update gameInProgress_ to false, to stop the loop
  gameInProgress_ = false;
  // if the computer has beaten the user, display a message to the loser
  if (player.isComputer() && !opponent.isComputer()) {
    cout << "\nOh no, you've lost! Better luck next time...\n\n";
  // otherwise, display a message to the winner (which may be a computer)
  } else {
    cout << "\nCongratulations " << player.name();
    cout << "! You've won in " << to_string(player.shotsTaken()) << " shots!\n\n";
  }
  // briefly pause before prompting the user to continue
  pause();
  promptToContinue();
  return true;
}

/** Blocks further execution for PAUSE_IN_SECONDS seconds. */
void GameController::pause() {
  // convert seconds to milliseconds
  this_thread::sleep_for(chrono::milliseconds(PAUSE_IN_SECONDS * 1000));
}

/** Prompts the user to press return before continuing. */
void GameController::promptToContinue() {
  // ask the user what they'd like to do
  string response = "";
  cout << "\nPress 'Enter' to continue... ('r' to return to the main menu or 'q' to quit): ";
  getline(cin, response);
  // if the response begins with 'q' (or 'Q') then exit the program
  if (tolower(response[0]) == 'q') {
    quit();
  // if the response begins with 'r' (or 'R') then return to the main menu
  } else if (tolower(response[0]) == 'r') {
    menu();
  }
}

/** Quits the program. */
void GameController::quit() {
  // ask for confirmation
  string response = "";
  cout << "Are you sure? (Enter 'y' to confirm): ";
  getline(cin, response);
  if (tolower(response[0]) == 'y') {
    // if the response begins with 'y' (ignoring case), then exit the program
    cout << "\nGoodbye!\n";
    exit(EXIT_SUCCESS);
  }
}