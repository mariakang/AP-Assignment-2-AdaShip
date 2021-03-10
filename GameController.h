#pragma once

#include "Config.h"
#include "CoordinateConverter.h"
#include "Fleet.h"
#include "Player.h"

#define PAUSE_IN_SECONDS 1 // gives the user time to read console output

/** Creates and sets up games, controlling the flow of the overall program. */ 
class GameController {
  private:
    /** The number of board rows. */
    int rows_;
    /** The number of board columns. */
    int columns_;
    /** The number of hidden mines. */
    int mines_;
    /** Whether or not the computer's board is output to the console. */
    bool showComputerBoard_;
    /** Helper object to convert between strings and board locations. */
    CoordinateConverter converter_;
    /** The collection of boats each player is given. */
    Fleet fleet_;

    /** 
     * Launches a two player game with the given number of human players, and the given
     * game modes.
     */
    void launchGame(int numberOfHumanPlayers, bool salvoMode, bool minesMode);

    /** Launches an experiment to compare random selection with the targeting algorithm. */
    void launchExperiment();

    /** 
     * Sets up the given player's board.
     *
     * Returns whether or not the action completed successfully.
     */
    bool gameSetup(Player& player, bool minesMode);

    /** 
     * Randomly places mines in the given player's board.
     *
     * Returns whether or not the action completed successfully.
     */
    bool placeMines(Player& player);
    
    /** 
     * Runs when the given player takes a turn against their opponent.
     *
     * When the turn is complete, if the player has won, the 'gameEnd' method will be
     * called. Otherwise, the user will be prompted to continue (or quit), and either
     * the program will be exited, or the next turn sequence automatically launched.
     *
     * Returns whether or not the action completed successfully.
     */
    bool takeTurns(Player& player, Player& opponent, bool salvoMode);

    /** 
     * Increments the number of shots taken by the given player, and prints their
     * target board to the console.
     *
     * Runs immediately after 'player' has fired a torpedo at 'opponent'.
     */
    void postTorpedoRoutine(Player& player, Player& opponent);

    /** 
     * Displays a message and prompts the user to continue.
     *
     * Runs when the given player has beaten their opponent.
     *
     * Returns whether or not the action completed successfully.
     */
    bool gameEnd(Player& player, Player& opponent);

    /** Blocks further execution for PAUSE_IN_SECONDS seconds. */
    void pause();

    /** 
     * Prompts the user to press return before continuing.
     *
     * Gives the user the option to return to the main menu or
     * quit the program.
     */
    void promptToContinue();

    /** Exits the program (upon receiving confirmation). */
    void quit();

    /** Returns a randomly generated number between 1 and 'upperBound' inclusive. */
    int randomNumber(int upperBound);

  public:
    /** Constructor method for GameController. */
    GameController(Config config);

    /** 
     * Displays the main menu and executes the chosen option, looping until the user
     * chooses to quit the program.
     */
    void menu();
};