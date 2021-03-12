# AdaShip
by Maria Kang

A live instance of this project is available on [Repl.it](https://repl.it/@mariakang/AdaShip)

## 1. Challenge Outline

### 1.1. Summary

‘AdaShip’ is a C++ computer console based adaptation of the classic two-player ‘Battleships’ game.

Each player is given a board divided into squares, and a collection of boats of various lengths. Each player then places their boats on their board, without revealing the boats’ locations to their opponent. Once both players have placed all of their boats, they take it in turns to select a square on their opponent’s board to fire a torpedo at. If the square is occupied by a boat, they are told it’s a ‘hit’, otherwise they are told it’s a ‘miss’. If all of the squares a boat occupies have been hit, the boat is sunk. The game ends when one player wins by sinking all of their opponent’s boats.

### 1.2. Implemented Features

The user is presented with a menu and asked to select an option.

![Screenshot 1](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-1.png)

#### 1.2.1. One player v computer

This is a two player game in which the first player is the user, and the second player is the computer. The game has three main phases: ‘setup’, ‘turn sequence’, and ‘game end’.

**Setup**

At the start of the game, the user is shown an empty game board and a list of boats. They are asked to place the boats on the board until all of them have been placed. They can place a single boat by entering a boat number, grid reference and orientation (v/h), or randomly place all of their remaining boats by pressing ‘Enter’. They are also given the option to reset the board.

![Screenshot 2](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-2.png)

![Screenshot 3](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-3.png)

If they select a boat which has already been placed, then it will be moved to the new position. If the boat cannot be placed in the location specified, then the user will be notified and asked to choose again.
Once all of their boats have been placed, the user is free to reposition them or reset the board as many times as they wish, until they hit ‘Enter’ to confirm the configuration. The computer will then automatically (randomly) place its own boats on its own board.

![Screenshot 6](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-6.png)

(The user can choose whether to show or hide the computer’s board; it is hidden in the example above, but shown by default.)

**Turn sequence**

Once both players’ boards have been set up, the players take it in turns to fire torpedoes at their opponent’s board. At the start of their turn, the user is shown a view of their own board (‘Boats’), and a restricted view of their opponent’s board (‘Targets’) in which only the ‘hits’ and ‘misses’ are displayed. They can then either enter the reference of the square on their opponent’s board which they would like to target, or just press ‘Enter’ to fire a torpedo randomly.

![Screenshot 9](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-9.png)

The user is told whether their shot was a ‘hit’ or a ‘miss’, and an updated view of their target board is displayed. Unlike most versions of battleships, in ‘AdaShip’, if a player sinks one of their opponent’s boats, they are notified immediately.

![Screenshot 10](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-10.png)


**Game end**

When a player sinks the last of their opponent’s boats, they immediately become the winner, and the game ends.

![Screenshot 11](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-11.png)

#### 1.2.2. Two player game

This is a two player game in which both players are users. The users are expected to share the same console and take turns. The game is essentially the same as the ‘one player v computer’ game above, replacing the computer with another human player.

#### 1.2.3. Salvo mode

‘Salvo’ mode is a variation of the game in which the number of shots a player has each turn is determined by the number of surviving boats they have in play (i.e. boats which have not been sunk yet). When it’s their turn, the user can enter the permitted number of targets in a single string (separated by spaces), or press ‘Enter’ to fire the permitted number of torpedoes at randomly chosen targets.

![Screenshot 12](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-12.png)

If the user enters too many targets, then the remainder above the permitted number will be ignored. If they enter fewer than the number allowed, then only those entered will be torpedoed.

#### 1.2.4. Hidden mines

In this variation of the game, once a player has confirmed their boat configuration at the end of the setup phase, a number of ‘hidden mines’ are automatically placed on their board in random locations. Each mine takes up one square, and whilst it cannot be placed in a square occupied by another mine, it may be placed in a square occupied by a boat.

![Screenshot 13](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-13.png)

During gameplay, if a torpedo is fired at a square containing a mine, then as well as the square itself being torpedoed, the 8 squares immediately surrounding it will ‘explode’. If any of the surrounding squares contain mines, then the squares immediately surrounding that mine will also explode, and so on.

![Screenshot 14](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-14.png)

#### 1.2.5. Customisation

The standard board size is 10 x 10, and the standard collection of boats is: Carrier (length: 5), Battleship (length: 4), Destroyer (length: 3), Submarine (length: 3) and Patrol Boat (length: 2). However, users can customise the configuration by specifying their preferences in the ‘adaship_config.ini’ file.
The program supports three types of customisation: board dimensions (up to a maximum 80 x 80), boats (up to a maximum of 80), and the number of hidden mines to use.

![Screenshot 15](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-15.png)

In addition to the above, when starting a game against the computer, users will be able to choose whether or not to display the computer’s board. (It is shown by default for testing purposes, but choosing to hide it makes the game considerably more interesting!) They will also be able to choose whether or not to increase the level of difficulty (see below).

#### 1.2.6. Improved targeting algorithm

By default, when it’s the computer’s turn, or a player chooses to ‘auto-fire’ their torpedoes, the targets are selected randomly. Any squares which have already been torpedoed will be excluded, but all other squares will have an equal probability of being chosen.

As mentioned above, when the user selects a ‘one player v computer’ game, they are given the option to increase the level of difficulty. If they choose to do this, then when it’s the computer’s turn, instead of selecting its targets randomly, the computer will use an enhanced targeting algorithm.

Furthermore, the main menu contains an option ‘Run experiment (random selection v targeting algorithm)’. When this option is selected, the user is prompted to enter the number of repetitions to use, and then for each repetition, a random boat configuration is set up, and the number of shots required to sink all of the ships using first random selection and then the targeting algorithm are output to the console. The average number of shots taken by each algorithm is displayed at the end.

Details on the implementation of the enhanced targeting algorithm are given below in section 3.

