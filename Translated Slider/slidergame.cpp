// Slider Game (Translated)
//
// Will Bahnmiller
// Created on: 5/22/2015

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_LEVELS 10   // Total number of levels, including tutorial
#define ROW_COUNT 20    // Height of each level
#define COL_COUNT 40    // Width of each level
#define SAV_COUNT 100   // Used to copy map of level

using namespace std;


class turtle {
    public:
        int col;        // current x coordinate
        int row;        // current y coordinate
        int nextcol;    // next move's x coordinate
        int nextrow;    // next move's y coordinate
        int direction;  // in degrees, using the unit circle
};


// Wait for user, then clear the console
void clearScreen() {
    cout << "\n";
    system("pause");
    system("cls");
}


// Convert int to string
string numToString(int n) {

    string ret;
    ostringstream convert;
    convert << n;
    ret = convert.str();
    return ret;

} // end of numToString


// Check if user input for a move is valid
bool isValid(string input) {

    char c = input.at(0);
    if ( (c == 'w') || (c == 'a') || (c == 's') || (c == 'd') || (c == 'q') || (c == 'e') )
        return true;

    return false;

} // end of isValid()


// Ask user to move, reset, or exit in a level
char getChoice() {

    string input;

    // Ask for input
    cout << "[w] Up  [a] Left  [s] Down  [d] Right  [e] Reset  [q] Exit   CHOICE: ";
    cin >> input;

    while ( !isValid(input) ) {
        cout << "Invalid input. Please try again.\n[w] Up  [a] Left  [s] Down  [d] Right  [e] Reset  [q] Exit   CHOICE: ";
        cin >> input;
    }

    return input.at(0);

} // end of getChoice()


// Turn some characters into ASCII characters for display
char charTranslate(char c) {

    if      (c == 'b') return 219;
    else if (c == 'V') return 31;
    else if (c == '<') return 17;
    else if (c == '>') return 16;
    else if (c == '^') return 30;
    else if (c == 's') return 15;
    else               return c;

} // end of charTranslate()


// Move the player based on input and environment
// 0 = Stopped movement, still alive
// 1 = Dead
// 2 = Goal
// 5 = Stopped movement, still alive, barriers removed
int move(turtle *player, char grid[ROW_COUNT+1][COL_COUNT+1]) {

    int i, j;
    char box = 219, slant2 = 92, gear = 15, up = 30, down = 31, right = 16, left = 17;
    bool foundTeleporter = false;

    // First, cleanup breadcrumbs
    for (i = 0; i < ROW_COUNT; i++) {
        for (j = 0; j < COL_COUNT; j++) {
            if (grid[i][j] == '.')
                grid[i][j] = ' ';
        }
    }

    // While the player is within the level boundary
    while (player->nextcol >= 0 && player->nextcol < 40 && player->nextrow >= 0 && player->nextrow < 20) {

        // Leave a breadcrumb at current location, if it is a blank space
        if (grid[player->row][player->col] == ' ')
            grid[player->row][player->col] = '.';

        // Box or barrier = stop immediately, still alive
        if (grid[player->nextrow][player->nextcol] == box || grid[player->nextrow][player->nextcol] == 'X')
            return 0;

        // Spike = stop immediately, player died
        else if (grid[player->nextrow][player->nextcol] == '+')
            return 1;

        // Goal = stop immediately, player won the level
        else if (grid[player->nextrow][player->nextcol] == '*')
            return 2;

        // Bumper (NW-SE) = move onto bumper and change direction
        else if (grid[player->nextrow][player->nextcol] == '/') {
            player->col = player->nextcol;
            player->row = player->nextrow;

            if (player->direction == 90) { player->nextcol++; player->direction = 0; }
            else if (player->direction == 180) { player->nextrow++; player->direction = 270; }
            else if (player->direction == 270) { player->nextcol--; player->direction = 180; }
            else { player->nextrow--; player->direction = 90; }
        }

        // Bumper (NE-SW) = move onto bumper and change direction accordingly
        else if (grid[player->nextrow][player->nextcol] == slant2) {
            player->col = player->nextcol;
            player->row = player->nextrow;

            if (player->direction == 90) { player->nextcol--; player->direction = 180; }
            else if (player->direction == 180) { player->nextrow--; player->direction = 90; }
            else if (player->direction == 270) { player->nextcol++; player->direction = 0; }
            else { player->nextrow++; player->direction = 270; }
        }

        // Move tile (down) = move player onto tile and change direction to down
        else if (grid[player->nextrow][player->nextcol] == down) {
            player->col = player->nextcol;
            player->row = player->nextrow;
            player->nextrow++;
            player->direction = 270;
        }

        // Move tile (left) = move player onto tile and change direction to left
        else if (grid[player->nextrow][player->nextcol] == left) {
            player->col = player->nextcol;
            player->row = player->nextrow;
            player->nextcol--;
            player->direction = 180;
        }

        // Move tile (right) = move player onto tile and change direction to right
        else if (grid[player->nextrow][player->nextcol] == right) {
            player->col = player->nextcol;
            player->row = player->nextrow;
            player->nextcol++;
            player->direction = 0;
        }

        // Move tile (up) = move player onto tile and change direction to up
        else if (grid[player->nextrow][player->nextcol] == up) {
            player->col = player->nextcol;
            player->row = player->nextrow;
            player->nextrow--;
            player->direction = 90;
        }

        // Move-through gate (vertical) = stop player if heading EW, move player onto gate otherwise
        else if (grid[player->nextrow][player->nextcol] == 'H') {
            if (player->direction == 0 || player->direction == 180)
                return 0;
            else {
                player->col = player->nextcol;
                player->row = player->nextrow;

                if (player->direction == 90) { player->nextrow--; }
                else if (player->direction == 180) { player->nextcol--; }
                else if (player->direction == 270) { player->nextrow++; }
                else { player->nextcol++; }
            }
        }

        // Move-through gate (horizontal) = stop player if heading NS, move player onto gate otherwise
        else if (grid[player->nextrow][player->nextcol] == 'I') {
            if (player->direction == 90 || player->direction == 270)
                return 0;
            else {
                player->col = player->nextcol;
                player->row = player->nextrow;

                if (player->direction == 90) { player->nextrow--; }
                else if (player->direction == 180) { player->nextcol--; }
                else if (player->direction == 270) { player->nextrow++; }
                else { player->nextcol++; }
            }
        }

        // Net = stop player on this spot; one time use each!
        else if (grid[player->nextrow][player->nextcol] == '#') {
            player->col = player->nextcol;
            player->row = player->nextrow;
            return 0;
        }

        // Switch = stop player on this spot; barriers will be removed
        else if (grid[player->nextrow][player->nextcol] == gear) {
            player->col = player->nextcol;
            player->row = player->nextrow;
            return 5;
        }

        // Teleporter = move player to the other teleporter on the map
        else if (grid[player->nextrow][player->nextcol] == '@') {
            for (i = 0; i < ROW_COUNT; i++) {
                for (j = 0; j < COL_COUNT; j++) {
                    if ( grid[i][j] == '@' && !(i == player->nextrow && j == player->nextcol) && foundTeleporter == false ) {
                        foundTeleporter = true;
                        player->col = j;
                        player->row = i;
                        if (player->direction == 90) { player->nextrow = i-1; player->nextcol = j;}
                        else if (player->direction == 180) { player->nextcol = j-1; player->nextrow = i;}
                        else if (player->direction == 270) { player->nextrow = i+1; player->nextcol = j;}
                        else { player->nextcol = j+1; player->nextrow = i;}
                        break;
                    }
                }
            }
        }

        // Empty spot or breadcrumb = move player normally
        else if (grid[player->nextrow][player->nextcol] == ' ' || grid[player->nextrow][player->nextcol] == '.') {
            player->col = player->nextcol;
            player->row = player->nextrow;

            if (player->direction == 90) { player->nextrow--; }
            else if (player->direction == 180) { player->nextcol--; }
            else if (player->direction == 270) { player->nextrow++; }
            else { player->nextcol++; }
        }

        // Error detection. New version of Slider changes unknown characters into spaces.
        else {
            cout << "\nENCOUNTERED UNKNOWN OBJECT: " << grid[player->nextrow][player->nextcol] << "\n\n";
            system("pause");
            return 0;
        }

        foundTeleporter = false;

    } // end of while

    return 0; // should never reach here

} // end of move()


// Play the selected level
void playLevel(int level) {

    int i, j, turn = 0, x, y, status = 0;
    bool win = false, quit = false;
    char grid[ROW_COUNT+1][COL_COUNT+1], superGrid[ROW_COUNT+4][SAV_COUNT+1], saveGrid[ROW_COUNT+4][SAV_COUNT+1];
    char choice = 'G', happy = 1;
    string line;
    turtle player;

    // Prepare level file
    string myLevel = "levels/" + numToString(level) + ".txt";
    ifstream levelFile;
    levelFile.open(myLevel.c_str());

    // Begin playing
    if (levelFile.is_open()) {

        // Print header info
        getline(levelFile, line);
        cout << line << "\n";
        printf(" ::TURN %2d::         | PLAYER = {-,-}\n", turn);
        strcpy(superGrid[0], line.c_str());

        // Get blank line
        getline(levelFile, line);
        strcpy(superGrid[1], line.c_str());

        // Top of grid
        getline(levelFile, line);
        cout << line << "\n";
        strcpy(superGrid[2], line.c_str());

        // For each row...
        for (i = 0; i < ROW_COUNT; i++) {

            getline(levelFile, line);
            strcpy(superGrid[i+3], line.c_str());
            cout << "|";

            // ...and for each column...
            for (j = 0; j < COL_COUNT; j++) {

                // ...print the map
                grid[i][j] = charTranslate( line.at(j+1) );
                superGrid[i+3][j+1] = grid[i][j];
                cout << grid[i][j];

                // Determine starting location
                if (grid[i][j] == 'O') {
                    player.col = j;
                    player.row = i;
                    x = j;
                    y = i;
                }
            }

            cout << "|\n";
        }

        // Bottom of grid
        getline(levelFile, line);
        cout << line << "\n";
        strcpy(superGrid[i+3], line.c_str());
        levelFile.close();              // done printing map

        // Save the grid
        for (i = 0; i < ROW_COUNT+4; i++)
            strcpy(saveGrid[i], superGrid[i]);

        // Ask user for input
        choice = getChoice();
        system("cls");

        // Continue playing until win or quit
        while (!win && !quit) {

            // Quit
            if (choice == 'q') {
                quit = true;
                status = -1;
            }

            // Reset
            else if (choice == 'e') {

                // Copy save to grids
                for (i = 0; i < ROW_COUNT+4; i++)
                    strcpy(superGrid[i], saveGrid[i]);
                for (i = 0; i < ROW_COUNT; i++) {
                    for (j = 0; j < COL_COUNT; j++)
                        grid[i][j] = saveGrid[i+3][j+1];
                }

                // Reset starting position and turn counter
                player.col = x;
                player.row = y;
                turn = -1;
            }

            else if (choice == 'w' || choice == 'a' || choice == 's' || choice == 'd') {

                // Move the player
                grid[player.row][player.col] = ' ';

                if (choice == 'w') {
                    player.direction = 90;
                    player.nextcol = player.col;
                    player.nextrow = player.row-1;
                }
                else if (choice == 'a') {
                    player.direction = 180;
                    player.nextcol = player.col-1;
                    player.nextrow = player.row;
                }
                else if (choice == 's') {
                    player.direction = 270;
                    player.nextcol = player.col;
                    player.nextrow = player.row+1;
                }
                else if (choice == 'd') {
                    player.direction = 0;
                    player.nextcol = player.col+1;
                    player.nextrow = player.row;
                }

                // Try to move the player in the given direction
                status = move(&player, grid);

                // Player reached the goal
                if (status == 2) {
                    grid[player.nextrow][player.nextcol] = 1;
                    grid[player.row][player.col] = '.';
                    win = true;
                }

                // Player reached switch, so replace barriers with blank spots
                else if (status == 5) {
                    for (i = 0; i < ROW_COUNT+1; i++) {
                        for (j = 0; j < COL_COUNT+1; j++) {
                            if (grid[i][j] == 'X')
                                grid[i][j] = ' ';
                        }
                    }
                    grid[player.row][player.col] = 'O';
                    status = 0;
                }

                // Otherwise move player icon to position
                else
                    grid[player.row][player.col] = 'O';
            }

            // Handle next turn
            if (choice != 'q') {

                // Print resulting map
                for (i = 0; i < ROW_COUNT; i++) {
                    for (j = 0; j < COL_COUNT; j++) {
                        superGrid[i+3][j+1] = grid[i][j];
                    }
                }
                for (i = 0; i < ROW_COUNT+4; i++) {
                    if (i == 1 && win == true)
                        printf("\n ::TURN %2d::         | PLAYER = {%d,%d}", turn+1, player.nextrow, player.nextcol);
                    else if (i == 1 && win == false)
                        printf("\n ::TURN %2d::         | PLAYER = {%d,%d}", turn+1, player.row, player.col);
                    else
                        cout << "\n" << superGrid[i];
                }
                cout << "\n";
            }

            // Get next input choice
            if (status == 0) {
                choice = getChoice();
                system("cls");
            }

            // Died, so reset map
            else if (status == 1) {
                cout << "You have died. Resetting the map.   ";
                system("pause");
                choice = 'e';
                turn = -1;
                status = 0;
            }

            // Increment turn counter
            turn++;

        } // won or quit

        // Won a level that was not the final level
        if (win == true && level < NUM_LEVELS-1) {
            cout << "Completed Level " << level << " in " << turn << " turns!   ";
            system("pause");
            system("cls");
            playLevel(level+1);
        }

        // Won the final level
        else if (win == true) {
            cout << "\nCONGRADULATIONS!\n\nYou have finished the final level! THANKS FOR PLAYING!  " << happy << "\n";
            clearScreen();
        }

    } // end of if is_open
    else
        cout << "\nCan't open file: '" << myLevel << "'!\n\n";

} // end of playLevel()


// Check if user input matches a level's passcode
int checkPassword(int input) {

    int code, temp, i;
    ifstream passFile;

    // Open passcodes file
    passFile.open("passwords.txt");

    // Scan the file
    if (passFile.is_open()) {
        for (i = 0; i < NUM_LEVELS; i++) {

            passFile >> temp;   // Level number
            passFile >> code;   // Passcode for this level

            // Check for a match
            if (code == input) {
                cout << "\nPassword matched for level " << i << "!\n\n";
                return i;
            }
        }
        passFile.close();
    }
    else
        cout << "\nCan't open passwords.txt!\n\n";

    return -1; // No match

} // end of checkPassword()


// Enter a password
void enterPassword() {

    int input, level;

    // Prompt user to enter a password
    cout << "Please enter a five-digit password or enter [0] to cancel: ";
    cin >> input;

    // Check the password to see if it's valid
    if (input != 0) {
        level = checkPassword(input);
        if (level == -1)
            cout << "\nSorry, but this password is incorrect. Returning to main menu.\n\n";
        else
            playLevel(level);
    }

} // end of enterPassword()


// Tutorial text
void tutorial() {

    // Explain how the game works
    system("cls");
    cout << "Let's go over the basics of Slider.\n";
    cout << "\nThe objective of the game is to move your ball ( O ) into the goal ( * ).\n";
    cout << "\nBut movement is not one space at a time. Instead, your ball will slide\n";
    cout << "until it hits something.\n";
    cout << "\nThere are many different objects you may run into. Check tutorial.txt to\n";
    cout << "learn about them.\n";
    cout << "\nNow, let's start with an easy map. All you have to do is go down ( 's' ) \nand right ( 'd' ).\n\n";
    clearScreen();

    // Load tutorial level
    playLevel(0);

} // end of tutorial()


// Begin a new game
void newGame() {

    string tut;

    // Ask if user wants to play tutorial or not
    do {

        cout << "  Would you like to play the tutorial?\n   [1] Yes\n   [2] No\n   [0] Cancel\n\n  CHOICE: ";
        cin >> tut;
        cout << "\n";

        // Send user to tutorial or first level
        if (tut.at(0) == '1') {
            tutorial();
            tut = "0";
        }
        else if (tut.at(0) == '2') {
            playLevel(1);
            tut = "0";
        }
        else if (tut.at(0) == '0') {
            cout << "Cancelling. Returning to main menu.\n\n";
            tut = "0";
        }
        else {
            cout << "Invalid input. Please try again.\n\n";
            clearScreen();
        }

    } while (tut.at(0) != '0');

} // end of newGame()


int main(void) {

    string menu;

    // Menu loop
    do {

        // Print menu info
        cout << "Welcome to Slider!\n\n*MENU*\n [1] New Game\n [2] Password\n [0] Exit\n\nCHOICE: ";
        cin >> menu;
        cout << "\n";

        // Determine action based on user's choice
        if (menu.at(0) == '1')
            newGame();
        else if (menu.at(0) == '2')
            enterPassword();
        else if (menu.at(0) != '0')
            cout << "Invalid input. Please try again.\n\n";

        if (menu.at(0) != '0')
            clearScreen();

    } while (menu.at(0) != '0');

    cout << "***Goodbye!***\n";
    return 0;

} // end of main
