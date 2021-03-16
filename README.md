# AdaShip
by Maria Kang
 
A live instance of this project is available on [Repl.it](https://repl.it/@mariakang/AP-Assignment-2-AdaShip)
 
## 1. Challenge Outline
 
### 1.1. Summary
 
‘AdaShip’ is a C++ computer console based adaptation of the classic two-player ‘Battleships’ game.
 
Each player is given a board divided into squares, and a collection of boats of various lengths. Each player then places their boats on their board, without revealing the boats’ locations to their opponent. Once both players have placed all of their boats, they take it in turns to select a square on their opponent’s board to fire a torpedo at. If the square is occupied by a boat, they are told it’s a ‘hit’, otherwise they are told it’s a ‘miss’. If all of the squares a boat occupies have been hit, the boat is sunk. The game ends when one player wins by sinking all of their opponent’s boats.
 
### 1.2. Implemented Features
 
The user is presented with a menu and asked to select an option.
 
![Screenshot 1](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-1.png)
 
#### 1.2.1. One player v computer
 
This is a two player game in which the first player is the user, and the second player is the computer. The game has three main phases:
- Setup
- Turn sequence
- Game end
 
**Setup**
 
At the start of the game, the user is shown an empty game board and a list of boats. They are asked to place the boats on the board until all of them have been placed. They can place a single boat by entering a boat number, grid reference and orientation (v/h), or randomly place all of their remaining boats by pressing ‘Enter’. They are also given the option to reset the board.
 
![Screenshot 2](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-2.png)
 
![Screenshot 3](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-3.png)
 
If they select a boat which has already been placed, then it will be moved to the new position. If the boat cannot be placed in the location specified, then the user will be notified and asked to choose again.

![Screenshot 4](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-4.png)

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
 
![Screenshot 5](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-5.png)
 
During gameplay, if a torpedo is fired at a square containing a mine, then as well as the square itself being torpedoed, the 8 squares immediately surrounding it will ‘explode’. If any of the surrounding squares contain mines, then the squares immediately surrounding that mine will also explode, and so on.
 
![Screenshot 14](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-14.png)
 
#### 1.2.5. Customisation
 
The standard board size is 10 x 10, and the standard collection of boats is:
- Carrier (length: 5)
- Battleship (length: 4)
- Destroyer (length: 3)
- Submarine (length: 3)
- Patrol Boat (length: 2)
 
However, users can customise the configuration by specifying their preferences in the ‘adaship_config.ini’ file.
 
The program supports three types of customisation: board dimensions (up to a maximum 80 x 80), boats (up to a maximum of 80), and the number of hidden mines to use.
 
![Screenshot 15](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-15.png)
 
In addition to the above, when starting a game against the computer, users will be able to choose whether or not to display the computer’s board. (It is shown by default for testing purposes, but choosing to hide it makes the game considerably more interesting!) They will also be able to choose whether or not to increase the level of difficulty (see below).
 
#### 1.2.6. Improved targeting algorithm
 
By default, when it’s the computer’s turn, or a player chooses to ‘auto-fire’ their torpedoes, the targets are selected randomly. Any squares which have already been torpedoed will be excluded, but all other squares will have an equal probability of being chosen.
 
As mentioned above, when the user selects a ‘one player v computer’ game, they are given the option to increase the level of difficulty. If they choose to do this, then when it’s the computer’s turn, instead of selecting its targets randomly, the computer will use an enhanced targeting algorithm.
 
Furthermore, the main menu contains an option ‘Run experiment (random selection v targeting algorithm)’. When this option is selected, the user is prompted to enter the number of repetitions to use, and then for each repetition, a random boat configuration is set up, and the number of shots required to sink all of the boats using first random selection and then the targeting algorithm are output to the console. The average number of shots taken by each algorithm is displayed at the end.
 
Details on the implementation of the enhanced targeting algorithm are given below in [section 3.3](#33-enhanced-targeting-algorithm).
 
### 1.3. Flow Diagram
 
The following [AdaShip Flow Diagram](https://maria-kang.com/photos/adaship/AdaShip-Flow-Diagram.png) illustrates how the program works:
 
![AdaShip Flow Diagram](https://maria-kang.com/photos/adaship/AdaShip-Flow-Diagram.png)
 
### 1.4. Development Plan
 
The overall development strategy was agile, with an iterative approach to design and implementation, early adoption of testing, and gradual addition of features. However, given the very short timeframe, it was deemed more efficient to 'think ahead' and incorporate some of the more enhanced features into the initial design, as opposed to creating an end-to-end minimum viable product (MVP) and only adding non-essential features once the MVP had been completed.
 
For example, the 'salvo' and 'hidden mines' modes were taken into consideration from the start when designing the torpedo firing algorithm, so as to ensure the code would not need to be completely re-written in order to add these features. Also, the board dimensions and boat specifications (including the total number of boats) were assumed to be configurable from the start, as this strongly influenced the overall class design (for example, requiring `Boat` objects and resizable `Fleet` and `Board` objects).
 
On the other hand, rendering the hidden mines and switching from a random to an enhanced targeting algorithm were features which were added after an initial end-to-end solution had been implemented and tested. Also, testing was carried out at the earliest possible stage during the implementation of each feature. Early testing ensures that any potentially design-altering issues are detected before the creation of large amounts of code which will subsequently need to be rewritten. Also, testing individual elements reduces the number of potential causes of any bugs, thereby making them quicker to diagnose and fix.
 
The development plan consisted of the following high level tasks, or 'epics':
 
- [Object-orientated design](#22-object-orientated-design)
- [Boat Placement and Torpedo Algorithms](#23-boat-placement-and-torpedo-algorithms)
- [Parsing the Configuration File](#24-parsing-the-configuration-file)
- [User Interface Implementation](#25-user-interface-implementation)
- [Additional Features](#26-additional-features)
 
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
 
Copying large sections of code should be avoided wherever possible by extracting them into reusable functions. Apart from taking up less space and making the code easier to read, this makes it much easier to maintain the code by ensuring that future changes only need to be made in one place instead of many. It also makes it easier for any new features wishing to use it.
 
### 2.2. Object-orientated Design
 
Using an object-orientated approach to design, the project was modelled as a collection of individual elements, each of which would be written as a class. Each class consists of members; attribute members or fields containing data, and function members or methods responsible for providing a specific area of functionality.
 
The first phase of development involved sketching out the class architecture by identifying:
- What the individual elements of the solution were, for example, `Player`, `Board`, `BoardSquare`, `Boat`, `BoardPrinter` etc.
- What pieces of information they would need to hold, for example, board dimensions, boat names and lengths, whether or not a square has been torpedoed etc.
- What functions they would need to perform, for example, calculating the maximum length of boat which could be placed in each square horizontally or vertically, printing a visual representation of the board to the console, etc.
 
After sketching out an initial design, the classes were coded up in a minimal way (to enable the next stage of algorithm design and implementation to begin) and then continuously updated and amended throughout the development process as their requirements became clearer. Additional classes were also created as part of various refactoring exercises (for example creating a separate `CoordinateConverter` class), making the overall object-orientated design an iterative process.
 
I used the `main.cpp` file to write temporary tests during the early stages of development, so as to ensure that the classes and members were behaving as expected. Being new to C++, I found this immensely helpful, as it enabled me to verify that my theoretical solution worked in practice. It also helped me to understand a lot of new concepts, such as when to use object references instead of values, when to overload the assignment operator, and the consequences of not doing so.
 
At the end of the project, once all of the functionality had been implemented and tested, an internal 'tidy up' operation was carried out. This involved taking each class in turn and doing the following:
- Removing any unused class members
- Making each member private unless used by another class
- Adding formal documentation above the declaration of each member
 
The following [AdaShip Class Diagram](https://maria-kang.com/photos/adaship/AdaShip-Class-Diagram.png) shows all of the classes involved, their members, and how they interact with each other. Members above the line are attribute members, those below the line are function members, and member visibility is denoted by a '-' for private, or '+' for public. The relationships between classes are denoted by arrows, and include dependency, aggregation, and composition.
 
![AdaShip Class Diagram](https://maria-kang.com/photos/adaship/AdaShip-Class-Diagram.png)
 
For further details on the behaviour and purpose of each class and member, please refer to the documentation which immediately precedes each class and member declaration.
 
### 2.3. Boat Placement and Torpedo Algorithms
 
Once an initial class structure had been sketched out, I turned my attention to algorithms. The two main algorithms which the gameplay would rely on, were the boat placing algorithm and the torpedo algorithm.
 
The design and implementation of the boat placing algorithm was broken down into the following tasks:
 - Convert a reference string into a board coordinate
 - Validate board coordinates
 - Calculate the maximum boat lengths which can be placed in each square vertically and horizontally, and figure out the most efficient way to update these each time a boat is placed
 - Design a system for accessing the relevant elements (e.g. `Player#getBoat(int boatId)`, `Player#getBoard()#getSquare(Coordinate c)`
 - Update the maximum boat lengths when a boat is removed (i.e. when the user wishes to place a boat which has already been placed)
 - Remove a given boat from the board
 - Place a given boat in the given orientation at the given coordinate
 - Generate a random number
 - For a given boat length and orientation, pick a random coordinate at which the boat could be placed
 - Place a given boat in a random orientation at a random coordinate
 - Place all remaining unplaced boats on the board in random orientations at random coordinates
 
The design and implementation of the torpedo firing algorithm was broken down into the following tasks:
 - Convert a reference string into a board coordinate
 - Validate board coordinates
 - Check whether a square has already been torpedoed
 - Fire a torpedo at a given coordinate
 - If a ‘hit’, update the boat’s damage and check if it’s been sunk
 - Generate a random number
 - Pick a random coordinate that has not yet been torpedoed
 - Fire a torpedo at a randomly chosen coordinate
 - If a ‘mine’, explode the surrounding squares
 
Since some of the tasks overlapped, for example the coordinate conversion and validation, and the random number generation, the code was designed to be as reusable as possible. This meant separating out the coordinate conversion and validation logic into separate functions which both algorithms could call.
 
Each of these algorithms could also be built up in layers. For example, for the boat placing algorithm, first a function to remove a boat was implemented. A function to place a given boat at a given location (and orientation) was then implemented, which involves a call to the removal function if the boat in question has already been placed. Next, a function to place a given boat at a random location was implemented by picking a random location and orientation, and then calling the previous function to place the boat there. Finally, a function to place all remaining boats was implemented by simply looping over the boats, calling the previous function in turn to place any unplaced boats in random locations.
 
I tested as early as possible, writing temporary basic tests which just ran parts of the code and printed some output to the console. The main purpose of these was to be able to check that the code compiled, enabling me to address any syntax and compilation issues early on. They also helped to verify that certain aspects were working as expected (for example the coordinate conversion functions).
 
However, without having created any of the UI components which would be necessary to test these algorithms properly, by the end of this phase, the code was only of prototype quality.
 
The ‘hidden mines’ variant of the game was factored into the initial design of the torpedo firing algorithm. However, the enhanced targeting algorithm was deemed out of scope for this phase, and scheduled to be done after an end-to-end MVP had been produced, if time permitted. The option to reset the board during the boat setup phase was also added at a later stage.
 
### 2.4. Parsing the Configuration File
 
The next phase of development involved dealing with the configuration file. Tasks included the following:
 - Parse an external file (open, read, close)
 - Read and store the board dimensions
 - Read and store each boat
 
The first iteration of this implementation involved making some strong assumptions about the structure of the user-provided configuration file. For example, it was assumed that the first line would contain the board dimensions, and each subsequent line would contain a boat specification. It was also assumed that each colon would be followed by a space.
 
These assumptions resulted in a code smell. My code treated the first line differently to the rest of the lines which - apart from looking a bit nasty - felt fragile and unable to withstand minor changes to the format of the config file. Treating the rest of the lines as boats was also a foolish assumption which failed to take account of potential blank lines, and left insufficient flexibility to add new categories in future (such as the number of mines, which was added in a later phase).

The code was therefore refactored to dynamically check which category (if any) each line related to before parsing it any further. The treatment of whitespace was also handled more robustly. Some assumptions still remain in place, for example the presence of colons after category headings, and commas after boat names, but I feel these are reasonable.
 
### 2.5. User Interface Implementation
 
The UI design and implementation phase included the following tasks:
 - Print the boats board to the console
 - Print the targets board to the console
 - Display the boats key
 - Display the main menu
 - Launch a game
 - UI to set up the board
 - UI to fire torpedoes
 - Setup and turn sequences
 - Game end sequence
 - Pause briefly to improve the gameflow
 - Prompt the user to press a key to continue
 - Quit the program
 
Since the board would need to be printed to the console at various times throughout the game (both the ‘boats’ and ‘targets’ views), I felt it made sense to separate this feature out into its’ own `BoardPrinter` class. Besides being easy to find and update, this made the code easily reusable.
 
I wrote some simple temporary tests early on to verify that the board printing functions were working as expected, and used the results to iteratively make changes to the board design (colours etc.) until I was happy with the way it looked.
 
Once all of the above tasks had been completed, I was able to perform more thorough testing by walking through the workflow as a user. Most features worked as expected, though some issues did come to light which needed to be fixed.
 
**Random number seed**

The first notable issue was to do with the way random numbers were generated. I’d initially created a reusable function to generate a random number, which also seeded the random number sequence first. This seeding is necessary, but should only be done once, not each time a random number is generated, particularly if the numbers are generated in short succession (since the seeding uses a timestamp). I first noticed this issue when I was testing randomly placing all boats, and spotted that all of the boats always had the same orientation. After looking into it, I realised that it could be fixed by moving the ‘seeding’ part out of the random number function and running it once for the whole program.
 
**Segmentation fault**

Another issue which had to be fixed was the occurrence of ‘segmentation faults’. While manually testing the game sequence, the program would occasionally crash, citing a ‘segmentation fault’. I researched what this meant, but the suggestion that it was somehow related to a memory access issue seemed unlikely, as it was occurring in the middle of the game, while performing an action which had previously been performed without a problem. There wasn’t any obvious debug logging I could add to help diagnose the issue, but after some more research, I hypothesised that it might have something to do with recursion.
 
I had initially written the turn sequence recursively. The function was called with ‘player’ and ‘opponent’ parameters, and towards the end of it, if no winner had been found, it would make a call to itself with the player and opponent parameters switched around. This meant that the turn sequence would keep alternating between the two players until a winner had been found. However, it occurred to me that perhaps there was a limit on the permitted depth of recursion. I tested this hypothesis by playing the game and counting how many times the recursive function was called before the game crashed. When I discovered that it was consistently crashing after the same number of turns (10), I deduced that this was the cause of the problem, and then rewrote the function to be iterative (using a while loop) instead of recursive. This fixed the problem.
 
### 2.6. Additional Features

Once a fully-functional, end-to-end product had been completed, the final phase (prior to internal tidy up) was to implement some additional features. These include:
 
 - Randomly place ‘hidden mines’ and update the board printer to display them
 - Implement an enhanced targeting algorithm
 - Allow the user to increase the difficulty level
 - Update the computer’s auto-fire logic to use the targeting algorithm if the difficulty has been increased
 - Add a ‘run experiment’ feature to test the new targeting algorithm
 - During setup, add the option to ‘reset’ the board
 - During setup, once all boats have been placed, ask the user to confirm the placement, allowing them to reposition boats until they do
 
Since the UI was already in place, testing these features as they were added was very straightforward. However, special care was taken to try to ensure that the new features weren’t introducing any unexpected regressions. It is impossible to guarantee this with manual testing, which is why in a production environment I’d expect to set up a test harness to run automated regression tests.
 
## 3. Evaluation
 
### 3.1. Advanced Programming Techniques
 
#### 3.1.1. Class architecture
 
When designing my object orientated solution to the problem, I attempted to separate out different types of functionality into different classes ('separation of concerns'). For example, classes such as `BoardSquare` and `Boat` are models which store information about an element of the game, whereas `BoardPrinter` and `BoatPlacer` are concerned with printing output to the console and implementing logic to update the model classes respectively.
 
Strictly speaking, my implementation doesn’t fit into a Model-View-Controller (MVC) framework, as the lines between view and controller are a little blurred. For example, the `GameController` class handles input and output, but also controls the flow of events. However, each class has been designed to perform a specific type of function, making it reasonably easy to identify which class is responsible for what.
 
When designing my class architecture, I tried to avoid making any classes too big or too small. If a class is allowed to become too big then it will be a burden to maintain, and similarly a large collection of very small classes is harder to maintain than a smaller collection of slightly larger ones. The `GameController` class originally handled the logic for placing boats and firing torpedoes, but these tasks were later delegated to `BoatPlacer` and `TorpedoLauncher` classes to make these easier to read and maintain. A future enhancement could be to refactor this further to separate out everything UI-related into a separate class.
 
#### 3.1.2. Avoiding global variables and static classes
 
I have used preprocessor definitions in my code (e.g. `MAX_ROWS` and `MAX_COLUMNS`) to improve readability, and aid maintenance by ensuring that hard coded values only need to be updated in one place. However, these are not variables; they act merely as a form of text replacement which is performed by the preprocessor before the program is compiled. I have chosen to avoid using global variables or static classes.
 
Global variables can be altered by any part of the code, making it difficult to remember or understand every possible use. A global variable can have no access control; it cannot be limited to only specific parts of the program. Using global variables causes very tight coupling of code, and testing can be an issue as it is difficult to decouple them when writing tests.
 
The main issue with global and static instances of classes is that the order in which they are constructed and destroyed depends on the implementation. This means that if the code relies on them being around at a certain time, this may lead to unexpected behaviour. Static classes act frequently as a global state and lead to non-deterministic behaviour which should be avoided. Also, code which uses static classes cannot be tested in isolation, making unit testing very difficult.
 
#### 3.1.3. Member visibility
 
Class members should always be given the minimum visibility necessary. If a member is public, then it means other classes could depend on it, therefore introducing a maintenance obligation. If the member is only part of the internal implementation of the class's public API, then it should be hidden from external view. This concept, known as information hiding or encapsulation, is fundamental to software design.
 
Encapsulation results in decoupled modules which can be developed, tested and optimised in isolation. This speeds up system development as independent modules can be developed in parallel. It also reduces the number of regression tests which need to be maintained, as only the behaviour of the public API needs to be verified; anything else is an implementation detail which can be modified without any unexpected consequences.
 
Therefore all class members were made private unless explicitly referenced by another class. Also, all attribute members were made private so as to protect their integrity. Getter methods were provided where necessary to allow external classes read-only access, and setter methods were provided under rarer circumstances, only if necessary.
 
#### 3.1.4. Defensive programming
 
Defensive programming is a technique used to make software behave in a predictable manner despite unexpected inputs or user actions. It involves minimising the number of assumptions made about the inputs received, and using guard statements to ensure that invalid inputs are handled gracefully.
 
AdaShip is an interactive game which relies on user input. Furthermore, since it is not possible to restrict what users can enter via the console-based UI, it is particularly important to check and validate the input before attempting to process it.
 
In some cases, the potential for error has been minimised by making it clear what the default behaviour will be, for example, '_(Enter 'y' or the board will be shown)_'. However in most cases, when user input is requested, the program will loop until valid input has been received. The input is validated, and if valid, it's processed and the loop is broken. Otherwise, a message is displayed to the user, and another loop begins.
 
Examples of input requiring this form of validation include:
- Main menu options
- Boat position requests
- Torpedo targets
- Experiment repetitions
 
The configuration file is also a potential source of error, though this is treated slightly differently. Instead of expecting the user to edit the file while the program is running, and looping until they do, the program simply outputs a message to say that there's an issue with it, and exits, giving the user a chance to fix the file offline.
 
Although user input is validated to ensure that errors are detected and handled gracefully, some user errors are predictable and therefore silently corrected to enhance the user's experience. Examples include:
- Making string comparisons case-insensitive
- Ignoring the rest of the string if the first part is valid
- Ignoring additional whitespace
- Handling some extra or missing whitespace in the configuration file
 
#### 3.1.5. User experience design
 
Care was taken to design a user interface which would be intuitive, easy and satisfying to use. This involved making instructions as clear as possible (for example by including sample input or 'greying out' placed boats), minimising the number of required interactions or key presses (for example by using 'Enter' for likely defaults), and trying to make the rendered boards aesthetically pleasing (for example using colours).
 
As mentioned above, some predictable user errors (such as case) are silently corrected so as to spare the user from doing any unnecessary extra work. Also, short pauses were added to improve the game flow by giving the user a chance to read whatever has just been output to the console.
 
### 3.2. Project Highlights
 
#### 3.2.1. Coordinate conversion
Throughout the program, there are instances when a grid reference in the form ‘A1’, ‘B2’ etc. needs to be converted into a coordinate.
 
One option could have been to store the board squares as a 1-dimensional map, mapping string references to `BoardSquare` objects. However, I felt that this approach would make the code harder to follow when it came to iterating over the board, or dealing with squares on neighbouring rows and columns. I felt it would be easier to follow if the rows and columns were expressed as integers, and the `BoardSquare` objects stored in a 2-dimensional array.
 
Therefore, in order to switch between the two, I created a `CoordinateConverter` class with methods to convert strings into coordinates, coordinates to strings, and validate whether or not a coordinate lies within the board’s boundaries. These methods can be called by any function wishing to use them.
 
The conversion process essentially works by using an algorithm to convert column numbers to Excel-style column headings, and populating a lookup map which can then be used to get the column number from its Excel-style column heading.
 
```
/** Populates the mapping from Excel-style column names to column numbers. */
void CoordinateConverter::initialiseLookup() {
 // populate an 'alpha to column number' lookup map
 for (int i = 1; i <= columns_; i++) {
   alphaLookup_[columnToString(i)] = i;
 }
}
 
/** Converts the given column number to its Excel-style representation. */
string CoordinateConverter::columnToString(int column) {
 const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 string alpha = "";
 // if the number is above 26, then an additional character (representing the
 // integer quotient when divided by 26) is prepended
 if (column > 26) {
   alpha += alphabet[(column - 1) / 26 - 1];
 }
 // the final character represents the remainder when divided by 26
 alpha += alphabet[(column - 1) % 26];
 return alpha;
} 
```
Note: Board rows and columns start at 1, whereas the `alphabet` string is 0-based.

Only the columns in the board’s range (as specified by the configuration file) are added to the map, and as all of these strings will be required by the board printing algorithm, no unnecessary mappings are added. Also, as this map is populated when the class is constructed, it means that the calculations (which have a time complexity of O(n)) only need to be performed once, as opposed to every time a method is called. The time complexity of looking up a value from the map is only O(1).
 
Since the function which converts a string into a coordinate needed to return a single object, I created a `Coordinate` class. This class simply provides a convenient way of grouping a row and column number together. I could have opted to add a `toString()` method on it, but I felt that a lookup map would be the most efficient way of performing this conversion, and it would have been inefficient to have to initialise this each time the function is called, or each time a `Coordinate` object is created.
 
Since reference strings entered by the user may or may not be valid, defensive programming has been used to check the validity of the string when trying to convert it to a coordinate. If the string is invalid, the function will return (0, 0), and as this isn’t a valid board coordinate, it will be easy for the function calling it to determine whether or not the string was valid. This was easier to work with than making the function throw an error.
 
#### 3.2.2. Boat placing algorithm
 
#### 3.2.3. Board rendering

The board rendering is the most prominent feature of the UI, so I tried to make it as aesthetically pleasing and clear as possible.

Key design features include:
 - Colour coding of squares (including fonts and backgrounds) to make them instantly clear
 - Use of symbols to add illustration (e.g. ‘~’ to represent a wave and ‘*’ to represent a splash)
 - Ensuring all squares are exactly the same width, regardless of contents (or column headings), so that the table is consistently aligned
 - ‘Greying out’ placed boats
 - Colour coding the boats key to indicate damage (i.e. green for undamaged, orange for hit and red for sunk)
 - Dynamically positioning the boats key at the side of the board or underneath it, depending on how wide the board is
 
Screenshots:
 
![Screenshot 3](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-3.png)
![Screenshot 1](https://maria-kang.com/photos/adaship/Screenshot-2021-03-16-1.png)
![Screenshot 5](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-5.png)
![Screenshot 2](https://maria-kang.com/photos/adaship/Screenshot-2021-03-16-2.png)
![Screenshot 9](https://maria-kang.com/photos/adaship/Screenshot-2021-03-11-9.png)

The implementation involved creating two main board printing functions: one for the 'boats' board, and another for the 'targets' board (the opponent's view). Given the obvious overlap between the two, I avoided unnecessary repetition by extracting reusable pieces of code into separate functions.

For example, I created a function to print the header rows (column headings and underline) which was used by both types of board. I also created a separate function to print the boats key, which in turn extracted printing a single line into its own function so that it could also be used in isolation to append a line of the key onto the end of a board row.

I also used parameters to be able to make slight alterations to functions rather than creating multiple versions of the same thing. For example, the only difference between the view of the user's board during setup, and the view of it during gameplay, is the colour coding of the boats key. Therefore it made sense to pass this in as a boolean parameter.

I decided to use preprocessor definitions to store formatting strings (colour codes etc.). The strings themselves are difficult to understand, so replacing them with human readable names makes the code much clearer to follow. Also, developing the colour scheme was an iterative process which involved several cycles of testing and 'tweaking'. Using preprocessor definitions which were all defined in one place at the top of the file made these updates much easier to make.

```
#define WAVE "\033[34;104m ~ \033[0m"
#define SPLASH "\033[1;97;104m * \033[0m"
#define HIT "\033[1;31;100m x \033[0m"
#define MINE "\033[1;35;104m M \033[0m"
```

As described [above](#312-avoiding-global-variables-and-static-classes), preprocessor definitions act as a form of text replacement; they are not global variables. 
 
### 3.3. Enhanced Targeting algorithm
 
As described above, the program includes an enhanced targeting algorithm for seeking out boats. The algorithm has two main components:
- **Prioritising squares adjacent to 'hits'**
In the classic game of battleships, if a square is hit, then unless the boat occupying it is of length 1 (which isn't the case in the standard setup), it means at least one of the adjacent squares must also contain a boat. It therefore makes sense to aim the next torpedo at one of these adjacent squares.
 
- **Calculating probability densities**
In essence, this involves calculating the relative probability of each square containing a boat, and then picking a square with the highest probability. For each boat, we iterate over the board, determining whether or not the boat can be placed there vertically or horizontally and (for each orientation) if it can, each square that it would occupy is given an additional probability weighting.
 
In the program, each time a square is 'hit', its adjacent squares (excluding diagonals and looking only within the boundaries of the board) are added to a stack of prioritised `targets_`. When the targeting algorithm is invoked, if the stack is non-empty, then items are popped off it until one which hasn't already been torpedoed is found, or the stack is empty. In the former case, the target in question is chosen. In the latter case, the probability density of each square is recalculated, and one with the highest probability is chosen as the target.
 
The program includes the option to run an experiment. For each repetition, boats are randomly placed on a board, and then the number of shots taken to sink all of the boats using the targeting algorithm are compared to the number of shots taken to sink the same configuration of boats using random selection.
 
Each time I've run the experiment with 1,000 repetitions, the targeting algorithm has consistently achieved an average of 55 shots, compared to the 95 shot average achieved by random selection.
 
![Screenshot 1](https://maria-kang.com/photos/adaship/Screenshot-2021-03-12-1.png)
![Screenshot 2](https://maria-kang.com/photos/adaship/Screenshot-2021-03-12-2.png)
 
As evidenced above, the targeting algorithm performs consistently better than random selection. I came up with it independently, and subsequent research suggests that this approach is sensible. However, it isn't as 'smart' as it could be.
 
In most standard versions of battleships, a player isn't told when an opponent's boat has been sunk. Since clustering boats together can be a valid obfuscation strategy, it would usually make sense for a player to check all adjacent targets before moving on. However, in AdaShip, players are notified whenever they sink an opponent's boat (including which boat it is), therefore making it possible to make further deductions regarding the potential positions of remaining boats.
 
In the interests of simplicity (and time!) this has not been taken into consideration when designing the algorithm. For example, prioritised targets are never removed from the stack without being torpedoed (unless they've already been torpedoed). Also, although it may be technically possible to deduce that a previously 'hit' square cannot contain part of a surviving boat, this is not taken into consideration when calculating relative probabilities (only 'misses' or board boundaries may prevent a boat from potentially being positioned in a given location).
 
### 3.4. Reflective Review
 
#### 3.4.1. Summary
 
Given the short timeframe and my own lack of experience with the language, I'm pleased with the final product. There are undoubtedly areas for improvement, for example separating out the UI from the logic which handles events, and refactoring the random number generation, but in general, I believe the program works as intended and the code is readable with clear documentation.
 
#### 3.4.2. Enhanced testing
 
The main area of weakness is testing. Manual tests were carried out as early as possible at all stages of development, and a number of issues fixed as a result. However, automated tests were out of scope, and exhaustive stress testing wasn't possible within the given timeframe.
 
In a production environment, it would be essential to write unit tests to verify the behaviour of key elements, and set up a test harness to ensure that these are automatically triggered each time a change is made. Changes made to one piece of code can often have unexpected consequences for another piece of code, making this type of automated regression testing important. Tests can also act as a form of contract for the code, making life easier for future developers tasked with maintaining it.
 
Manual stress testing should also form part of the standard quality assurance (QA) process. QA testing involves performing deliberately intense and thorough tests to see if it's possible for a user to break the program. While a lot of manual testing was carried out, it wasn't feasible to stress test every possible scenario and edge case, making it impossible to rule out the presence of bugs.
 
#### 3.4.3. Continued professional development
 
During the development of this project, I learnt a lot about C++ and object-orientated programming. However, I feel I've only just scratched the surface and there's a lot more to learn. For example, there are a lot of standard data structures (sets, deques, lists etc.) which I might have used if I'd been implementing this in another language, but which I'm unfamiliar with in a C++ context. I'd therefore be very interested to learn which data structures professional C++ developers would use in various scenarios and why.