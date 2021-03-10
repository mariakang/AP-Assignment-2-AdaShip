#pragma once

#include <iostream>
#include <string>

using namespace std;

#include "BoardSquare.h"
#include "Player.h"

/** Prints a formatted view of the player's board to the console. */
class BoardPrinter {
  private:
    /** Prints the first two rows of the board (headings and underline). */
    void printHeader(int columns);

    /** Prints the given occupied board square. */
    void printBoatSquare(BoardSquare square);

    /** Returns a formatted string representing the given boatId. */
    string displayId(int boatId);

    /** 
     * Prints a line of the boats key. 
     *
     * If setupMode is set to true, then placed boats will be greyed out;
     * if set to false, then boats will be colour-coded based on damage.
     */
    void printBoatsKeyLine(Player player, int boatId, bool setupMode);

    /** 
     * Prints the boats key. 
     *
     * If setupMode is set to true, then placed boats will be greyed out;
     * if set to false, then boats will be colour-coded based on damage.
     */
    void printBoatsKey(Player player, bool setupMode);
    
  public:
    /** Constructor method for BoardPrinter. */
    BoardPrinter() {}

    /** 
     * Prints the given player's own board, displaying their own boats.
     *
     * If setupMode is set to true, then placed boats will be greyed out;
     * if set to false, then boats will be colour-coded based on damage.
     * By default, it's set to false.
     */
    void printBoard(Player player, bool setupMode = false);

    /** Prints the opponent's view of the given player's board. */
    void printBoardOpponentView(Player player);
};