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

### 1.3. Flow Diagram

The following [AdaShip Flow Diagram](https://maria-kang.com/photos/adaship/AdaShip-Flow-Diagram.png) illustrates how the program works:

![AdaShip Flow Diagram](https://maria-kang.com/photos/adaship/AdaShip-Flow-Diagram.png)

### 1.4. Development Plan

The overall development strategy was agile, with an iterative approach to design and implementation, early adoption of testing, and gradual addition of features. However, given the very short timeframe, it was deemed more efficient to 'think ahead' and incorporate some of the more enhanced features into the initial design, as opposed to creating an end-to-end minimum viable product (MVP) and only adding non-essential features once the MVP has been completed.

For example, the 'salvo' and 'hidden mines' modes were taken into consideration from the start when desigining the torpedo firing algorithm, so as to ensure the code would not need to be completely re-written in order to add these features. Also, the board dimensions and boat specifications (including the total number of boats) were assumed to be configurable from the start, as this strongly influenced the overall class design (for example, requiring `Boat` objects and resizable `Fleet` and `Board` objects).

On the other hand, rendering the hidden mines and switching from a random to an enhanced targeting algorithm were features which were added after an initial end-to-end solution had been implemented and tested. Also, testing was carried out at the earliest possible stage during the implementation of each feature. Early testing ensures that any potentially design-altering issues are detected before the creation of large amounts of code which will subsequently need to be rewritten. Also, testing individual elements reduces the number of potential causes of any bugs, thereby making them quicker to diagnose and fix.

The development plan consisted of the following high level tasks, or 'epics':

 - [Object-orientated design] (#22-object-orientated-design)
 - [Ship Placement and Torpedo Algorithms] (#23-ship-placement-and-torpedo-algorithms)
 - [Parsing the Configuration File] (#24-parsing-the-configuration-file)
 - [User Interface Implementation] (#25-user-interface-implementation)
 - [Additional Features] (#26-additional-features)

Each of these stages are described in more detail below.

## 2. Development

### 2.1. Adoption of Good Standards

As many ‘good standards’ as possible were adopted throughout the development of this project, including the following:

#### 2.1.1. Naming conventions

All class names are defined in upper camel case, and all variable and function names in lower camel case. All attribute class member names end in a trailing underscore, and literal constant names are defined in upper snake case.

Having consistent naming conventions makes the code much easier to read, as it provides additional context. For example, beginning class names with a capital letter makes it easier to distinguish types from variables, and using a trailing underscore for attribute members makes them distinguishable from local variables or parameters. It also makes the code more aesthetically pleasing, and hence more readable.

In addition to consistency, names have been chosen to be as self-explanatory as possible. Whilst this may lead to unnecessary verbosity at times, using helpful names makes it much easier for other developers to understand the code when tasked with maintaining it in future.

#### 2.1.2. Code formatting

Code has been consistently formatted, for example using new lines and indentation. This includes lining up opening and closing curly braces, and ensuring that no line is too long (readers shouldn’t have to keep scrolling to the right).

Consistent formatting improves aesthetics and hence readability for other developers tasked with maintaining the code, but can also make it much easier to spot and correct syntax errors early on while developing it. For example, always ending a line with a semicolon makes it easier to spot any missing ones, and lining up opening and closing braces makes it easier to spot if these have been correctly matched off.

#### 2.1.3. Documentation

Fairly extensive documentation has been provided above each class member/function definition to make it clear what the member is and does.

Besides helping anyone wishing to maintain the code in future, documentation is potentially of great importance to end users. In this case, the end user shouldn’t need to look at the code at all, but if, for example, the product was a public API, then providing helpful documentation would be vital.

In addition to providing formal documentation, I’ve also included comments throughout the code to explain what’s going on at each stage. In a professional context, extensive use of commenting should be avoidable by making the code itself as self-explanatory as possible. Comments should not be needed to say what the code does, but rather why it does it. However, in an academic context, it is important to communicate clearly how algorithms are intended to work, so comments have been used extensively to this end.

#### 2.1.4. Class header files

Each class definition has been written in a separate header file (whose name matches the class name), with most function implementations being written in an accompanying ‘.cpp’ file.

Splitting the code out into separate files makes it much easier to navigate and find the code you’re looking for quickly. It also improves compile times during development, as the larger an individual file gets, the longer it takes to compile. If everything was in a single file, then the whole thing would have to be recompiled each time a change is made, however small. Using separate header files means only the files which have been edited need to be recompiled, potentially saving developers a lot of time.

Separating out the implementation into ‘.cpp’ files and keeping the content of the header files as simple as possible greatly improves readability, as you can see at a glance what the class does and contains.

#### 2.1.5. Code reuse

### 2.2. Object-orientated Design

Using an object-orientated approach to design, the project was modelled as a collection of individual elements, each of which would be written as a class. Each class consists of members; attribute members or fields containing data, and function members or methods responsible for providing a specific area of functionality.

The first phase of development involved sketching out the class architecture by identifying:
 - What the individual elements of the solution were, for example, `Player`, `Board`, `BoardSquare`, `Boat`, `BoardPrinter` etc.
 - What pieces of information they would need to hold, for example, board dimensions, boat names and lengths, whether or not a square has been torpedoed etc.
 - What functions they would need to perform, for example, calculating the maximum length of boat which could be placed in each square horizontally or vertically, printing a visual representation of the board to the console, etc.

After sketching out an initial design, the classes were coded up in a minimal way (to enable the next stage of algorithm design and implementation to begin) and then continuously updated and amended throughout the development process as their requirements became clearer. Additional classes were also created as part of various refactoring exercises (for example creating a separate `CoordinateConverter` class), making the overall object-orientated design an iterative process.

Once all of the functionality had been implemented and tested, an internal 'tidy up' operation was carried out. This involved taking each class in turn and doing the following:
 - Removing any unused class members
 - Making each member private unless used by another class
 - Adding formal documentation above the declaration of each member

The following [AdaShip Class Diagram](https://maria-kang.com/photos/adaship/AdaShip-Class-Diagram.png) shows all of the classes involved, their members, and how they interact with each other. Members above the line are attribute members, those below the line are function members, and member visibility is denoted by a '-' for private, or '+' for public. The relationships between classes are denoted by arrows, and include dependency, aggregation, and composition.

![AdaShip Class Diagram](https://maria-kang.com/photos/adaship/AdaShip-Class-Diagram.png)

For further details on the behaviour and purpose of each class and member, please refer to the documentation which immediately precedes each class and member declaration.

### 2.3. Ship Placement and Torpedo Algorithms

### 2.4. Parsing the Configuration File

### 2.5. User Interface Implementation

### 2.6. Additional Features

## 3. Evaluation

### 3.1. Code Refactoring

### 3.2. Advanced Programming Techniques

#### 3.2.1. Class architecture

When designing my object orientated solution to the problem, I attempted to separate out different types of functionality into different classes. For example, classes such as `BoardSquare` and `Boat` are models which store information about an element of the game, whereas `BoardPrinter` and `BoatPlacer` are concerned with printing output to the console and implementing logic to update the model classes respectively.

Strictly speaking, my implementation doesn’t fit into a Model-View-Controller (MVC) framework, as the lines between view and controller are a little blurred. For example, the `GameController` class handles input and output, but also controls the flow of events. However, each class has been designed to perform a specific type of function, making it reasonably easy to identify which class is responsible for what.

When designing my class architecture, I tried to avoid making any classes too big or too small. If a class is allowed to become too big then it will be a burden to maintain, and similarly a large collection of very small classes is harder to maintain than a smaller collection of slightly larger ones. The `GameController` class originally handled the logic for placing boats and firing torpedoes, but these tasks were later delegated to `BoatPlacer` and `TorpedoLauncher` classes to make these easier to read and maintain. A future enhancement could be to refactor this further to separate out everything UI-related into a separate class.

#### 3.2.2. Avoiding global variables and static classes

I have used literal constants in my code (e.g. `MAX_ROWS` and `MAX_COLUMNS`) to improve readability, and aid maintenance by ensuring that hard coded values only need to be updated in one place. However, I have chosen to avoid using global variables or static classes. 

Global variables can be altered by any part of the code, making it difficult to remember or understand every possible use. A global variable can have no access control; it cannot be limited to only specific parts of the program. Using global variables causes very tight coupling of code, and testing can be an issue as it is difficult to decouple them when writing tests.

The main issue with global and static instances of classes is that the order in which they are constructed and destroyed depends on the implementation. This means that if the code relies on them being around at a certain time, this may lead to unexpected behaviour. Static classes act frequently as a global state and lead to non-deterministic behaviour which should be avoided. Also, code which uses static classes cannot be tested in isolation, making unit testing very difficult.

#### 3.2.3. Member visibility

#### 3.2.4. Defensive programming

#### 3.2.5. User experience design

### 3.3. Project Highlights

### 3.4. Enhanced Targeting algorithm

![Screenshot 1](https://maria-kang.com/photos/adaship/Screenshot-2021-03-12-1.png)
![Screenshot 2](https://maria-kang.com/photos/adaship/Screenshot-2021-03-12-2.png)

### 3.5. Reflective Review
