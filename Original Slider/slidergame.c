// Slider Game
//
// Will Bahnmiller
// Created on: 12/16/2012

// IF YOU WANT TO ADD MORE LEVELS, CHANGE NUM_LEVELS AT ~LINE 26,
// ADD THE FILE INTO game AT ~LINE 392, AND ADD THE PASSWORD TO passwords.txt

/*
219 BOX
30  UP FILLED ARROW
31  DOWN FILLED ARROW
16  RIGHT FILLED ARROW
17  LEFT FILLED ARROW
1   HAPPY FACE
15  GEAR
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define NUM_LEVELS 5

// FILES
FILE *map, *codes;

// STRUCTS
struct turtle {

    int loc_col;     // current x coordinate
    int loc_row;     // current y coordinate
    int next_col;    // next move's x coordinate
    int next_row;    // next move's y coordinate
    int direction;   // in degrees, using the unit circle

};

// FUNCTIONS
void tutorial();  // Walks the Player through a basic level
int password(int input);  // Takes in a password and determines if it is valid
int move(struct turtle *player, char grid[21][41]);  // Moves the player and updates the map. 0 = alive, 1 = died, 2 = goal, 5 = switch
char getChoice();  // Asks for player action
int game(int level);  // The actual game, given which map we are on
                      // 0 = Player quit, return to main menu
                      // 1 or more = Player won, move onto next level


int main(void) {

    int menu = -1, tut = -1, input, level = 1, winCheck = 1, complete = 0;

    while (menu != 0) {

        printf("Welcome to Slider!\n\n*MENU*\n [1] New Game\n [2] Password\n [0] Exit\n\nCHOICE: ");
        scanf("%d", &menu);
        printf("\n");

        // New Game
        if (menu == 1) {

            printf("  Would you like to play the tutorial?\n   [1] Yes\n   [2] No\n   [0] Cancel\n\n  CHOICE: ");
            scanf("%d", &tut);
            printf("\n");

            if (tut == 1) {
                tutorial();
                level = 0;
                tut = 2;
                winCheck = 1;
                }

            if (tut == 2) {

                system("cls");

                while (level < NUM_LEVELS && winCheck != 0) {

                    complete = 0;
                    winCheck = game(level);
                    level++;

                    if (winCheck != 0) {
                        printf("Completed Level %d in %d turns!   ", level-1, winCheck);
                        system("pause");
                        system("cls");
                        }

                    if (winCheck != 0 && level == NUM_LEVELS) {
                        complete = 1;
                        printf("\nCONGRADULATIONS!\n\nYou have finished the final level! THANKS FOR PLAYING!  %c\n", 1);
                        }

                    }
                }

            else {
                printf("Cancelling. Returning to main menu.\n\n");
                }

            } // end of new game

        // Password
        else if (menu == 2) {

            printf("Please enter a five-digit password or enter [0] to cancel: ");
            scanf("%d", &input);

            if (input == 0) {
                }

            else if (password(input) == -1) {
                printf("\nSorry, but this password is incorrect. Returning to main menu.\n\n");
                }

            else {

                winCheck = 1;
                level = password(input);
                system("cls");

                while (level < NUM_LEVELS && winCheck != 0) {

                    complete = 0;
                    winCheck = game(level);
                    level++;

                    if (winCheck != 0) {
                        printf("Completed Level %d in %d turns!   ", level-1, winCheck);
                        system("pause");
                        system("cls");
                        }

                    if (winCheck != 0 && level == NUM_LEVELS) {
                        complete = 1;
                        printf("\nCONGRADULATIONS!\n\nYou have finished the final level! THANKS FOR PLAYING!  %c\n", 1);
                        }
                    }
                }

            } // end of password

        // Invalid
        else if (menu != 1 && menu != 2 && menu != 0) {
            printf("\nInvalid input. Please try again.\n\n");
            }

        if (menu != 0) {
            printf("\n");
            if (winCheck != 0)
                system("pause");
            system("cls");
            }

        } // end of menu = quit

    printf("***Goodbye!***\n");

    return 0;

}


void tutorial() {

    system("cls");

    printf("Let's go over the basics of Slider.\n");
    printf("\nThe objective of the game is to move your ball ( O ) into the goal ( * ).\n");
    printf("\nBut movement is not one space at a time. Instead, your ball will slide\n");
    printf("until it hits something.\n");
    printf("\nThere are many different objects you may run into. Check tutorial.txt to\n");
    printf("learn about them.\n");
    printf("\nNow, let's start with an easy map. All you have to do is go down ( 's' ) \nand right ( 'd' ).\n\n");
    system("pause");

    system("cls");

}

int password(int input) {

    int list[NUM_LEVELS], temp, i, value = -1;

    codes = fopen("passwords.txt", "r");

    for (i = 0; i < NUM_LEVELS; i++) {

        fscanf(codes, "%d", &temp);
        fscanf(codes, "%d", &list[i]);

        if (input == list[i])
            value = i;

        }

    return value;

}

int move(struct turtle *player, char grid[21][41]) {

    int i, j;
    char box = 219, slant2 = 92, gear = 15, up = 30, down = 31, right = 16, left = 17;

    while (player->next_col >= 0 && player->next_col < 40 && player->next_row >= 0 && player->next_row < 20) {

        // Box or barrier = stop immediately, still alive
        if (grid[player->next_row][player->next_col] == box || grid[player->next_row][player->next_col] == 'X')
            return 0;

        // Spike = stop immediately, player died
        else if (grid[player->next_row][player->next_col] == '+')
            return 1;

        // Goal = stop immediately, player won the level
        else if (grid[player->next_row][player->next_col] == '*') {
            return 2;
            }

        // Bumper (NW-SE) = move onto bumper and change direction
        else if (grid[player->next_row][player->next_col] == '/') {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;

            if (player->direction == 90) { player->next_col++; player->direction = 0;}
            else if (player->direction == 180) { player->next_row++; player->direction = 270;}
            else if (player->direction == 270) { player->next_col--; player->direction = 180;}
            else { player->next_row--; player->direction = 90;}
            }

        // Bumper (NE-SW) = move onto bumper and change direction accordingly
        else if (grid[player->next_row][player->next_col] == slant2) {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;

            if (player->direction == 90) { player->next_col--; player->direction = 180;}
            else if (player->direction == 180) { player->next_row--; player->direction = 90;}
            else if (player->direction == 270) { player->next_col++; player->direction = 0;}
            else { player->next_row++; player->direction = 270;}
            }

        // Move tile (down) = move player onto tile and change direction to down
        else if (grid[player->next_row][player->next_col] == down) {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;
            player->next_row++;
            player->direction = 270;
            }

        // Move tile (left) = move player onto tile and change direction to left
        else if (grid[player->next_row][player->next_col] == left) {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;
            player->next_col--;
            player->direction = 180;
            }

        // Move tile (right) = move player onto tile and change direction to right
        else if (grid[player->next_row][player->next_col] == right) {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;
            player->next_col++;
            player->direction = 0;
            }

        // Move tile (up) = move player onto tile and change direction to up
        else if (grid[player->next_row][player->next_col] == up) {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;
            player->next_row--;
            player->direction = 90;
            }

        // Move-through gate (vertical) = stop player if heading EW, move player onto gate otherwise
        else if (grid[player->next_row][player->next_col] == 'H') {
            if (player->direction == 0 || player->direction == 180)
                return 0;
            else {
                player->loc_col = player->next_col;
                player->loc_row = player->next_row;

                if (player->direction == 90) { player->next_row--; }
                else if (player->direction == 180) { player->next_col--; }
                else if (player->direction == 270) { player->next_row++; }
                else { player->next_col++; }
                }
            }

        // Move-through gate (horizontal) = stop player if heading NS, move player onto gate otherwise
        else if (grid[player->next_row][player->next_col] == 'I') {
            if (player->direction == 90 || player->direction == 270)
                return 0;
            else {
                player->loc_col = player->next_col;
                player->loc_row = player->next_row;

                if (player->direction == 90) { player->next_row--; }
                else if (player->direction == 180) { player->next_col--; }
                else if (player->direction == 270) { player->next_row++; }
                else { player->next_col++; }
                }
            }

        // Net = stop player on this spot; one time use each!
        else if (grid[player->next_row][player->next_col] == '#') {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;
            return 0;
            }

        // Switch = stop player on this spot; barriers will be removed
        else if (grid[player->next_row][player->next_col] == gear) {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;
            return 5;
            }

        // Teleporter = move player to the other teleporter on the map
        else if (grid[player->next_row][player->next_col] == '@') {
            for (i = 0; i < 20; i++) {
                for (j = 0; j < 40; j++) {
                    if (grid[i][j] == '@' && i != player->next_row && j != player->next_col) {
                        player->loc_col = j;
                        player->loc_row = i;
                        if (player->direction == 90) { player->next_row = i-1; player->next_col = j;}
                        else if (player->direction == 180) { player->next_col = j-1; player->next_row = i;}
                        else if (player->direction == 270) { player->next_row = i+1; player->next_col = j;}
                        else { player->next_col = j+1; player->next_row = i;}
                        }
                    }
                }
            }

        // Empty spot = move player normaly
        else if (grid[player->next_row][player->next_col] == ' ') {
            player->loc_col = player->next_col;
            player->loc_row = player->next_row;

            if (player->direction == 90) { player->next_row--; }
            else if (player->direction == 180) { player->next_col--; }
            else if (player->direction == 270) { player->next_row++; }
            else { player->next_col++; }
            }

        // Error detection. New version of Slider changes unknown characters into spaces.
        else {
            printf("\nENCOUNTERED UNKNOWN OBJECT: %c\n\n", grid[player->next_row][player->next_col]);
            system("pause");
            return 0;
            }

        }

    return 0;

}

char getChoice() {

    int error = 1;
    char c[100];

    printf("[w] Up  [a] Left  [s] Down  [d] Right  [e] Reset  [q] Exit   CHOICE: ");
    scanf("%s", c);

    if ( (c[0] != 'w') && (c[0] != 'a') && (c[0] != 's') && (c[0] != 'd') && (c[0] != 'q') && (c[0] != 'e') )
        error = 0;

    while (error == 0) {
        printf("Invalid input. Please try again.\n[w] Up  [a] Left  [s] Down  [d] Right  [e] Reset  [q] Exit   CHOICE: ");
        scanf("%s", c);
        if ( (c[0] == 'w') || (c[0] == 'a') || (c[0] == 's') || (c[0] == 'd') || (c[0] == 'q') || (c[0] == 'e') )
            error = 1;
        }

    return c[0];

}

int game(int level) {

         if (level == 0)   { map = fopen("level0.txt", "r"); }
    else if (level == 1)   { map = fopen("level1.txt", "r"); }
    else if (level == 2)   { map = fopen("level2.txt", "r"); }
    else if (level == 3)   { map = fopen("level3.txt", "r"); }
    else if (level == 4)   { map = fopen("level4.txt", "r"); }
    else if (level == 5)   { map = fopen("level5.txt", "r"); }
    else if (level == 6)   { map = fopen("level6.txt", "r"); }
    else if (level == 7)   { map = fopen("level7.txt", "r"); }
    else if (level == 8)   { map = fopen("level8.txt", "r"); }
    else if (level == 9)   { map = fopen("level9.txt", "r"); }

    int i, j, turn = 0, win = 0, quit = 0, x, y, status = 0;
    char grid[21][41], superGrid[24][101], saveGrid[24][101];
    char s[101];
    char choice = 'G';
    struct turtle player;

    // Print level info
    fgets(s, sizeof(s), map); // header
    printf("%s", s); strcpy(superGrid[0], s); printf(" ::TURN %2d::         | PLAYER = {-,-}\n", turn);
    fgets(s, sizeof(s), map); // blank
    strcpy(superGrid[1], s);
    fgets(s, sizeof(s), map); // top of grid
    printf("%s", s); strcpy(superGrid[2], s);

    for (i = 0; i < 20; i++) {

        fgets(s, sizeof(s), map);
        printf("%c", s[0]); strcpy(superGrid[i+3], s);

        for (j = 0; j < 40; j++) {

            grid[i][j] = s[j+1];

            if (grid[i][j] == ' ') { printf(" "); }
       else if (grid[i][j] == 'O') { printf("O"); player.loc_col = j; x = j; player.loc_row = i; y = i; }
       else if (grid[i][j] == '*') { printf("*"); }
       else if (grid[i][j] == '+') { printf("+"); }
       else if (grid[i][j] == 'b') { printf("%c", 219); grid[i][j] = 219; superGrid[i+3][j+1] = 219; }
       else if (grid[i][j] == '/') { printf("/"); }
       else if (grid[i][j] == 92 ) { printf("%c", 92 ); grid[i][j] = 92; superGrid[i+3][j+1] = 92;}
       else if (grid[i][j] == 'V') { printf("%c", 31 ); grid[i][j] = 31; superGrid[i+3][j+1] = 31;}
       else if (grid[i][j] == '<') { printf("%c", 17 ); grid[i][j] = 17; superGrid[i+3][j+1] = 17;}
       else if (grid[i][j] == '>') { printf("%c", 16 ); grid[i][j] = 16; superGrid[i+3][j+1] = 16;}
       else if (grid[i][j] == '^') { printf("%c", 30 ); grid[i][j] = 30; superGrid[i+3][j+1] = 30;}
       else if (grid[i][j] == '@') { printf("@"); }
       else if (grid[i][j] == 'H') { printf("H"); }
       else if (grid[i][j] == 'I') { printf("I"); }
       else if (grid[i][j] == '#') { printf("#"); }
       else if (grid[i][j] == 's') { printf("%c", 15 ); grid[i][j] = 15; superGrid[i+3][j+1] = 15;}
       else if (grid[i][j] == 'X') { printf("X"); }
       else    {grid[i][j] =  ' ';   printf(" "); }

            }

        printf("|\n");

        }

    fgets(s, sizeof(s), map); // bottom of grid
    printf("%s", s); strcpy(superGrid[i+3], s);
    // done printing map

    for (i = 0; i < 24; i++)
        strcpy(saveGrid[i], superGrid[i]);

    choice = getChoice();

    while (win == 0 && quit == 0) {

        system("cls");

        if (choice == 'q') {
            quit = 1;
            status = -1;
            }

        else if (choice == 'e') {

            // copy save to grids
            for (i = 0; i < 23; i++)
                strcpy(superGrid[i], saveGrid[i]);

            for (i = 0; i < 20; i++) {
                for (j = 0; j < 40; j++)
                    grid[i][j] = saveGrid[i+3][j+1];
                }

            player.loc_col = x;
            player.loc_row = y;
            turn = -1;
            }

        else if (choice == 'w' || choice == 'a' || choice == 's' || choice == 'd') {
            // move
            grid[player.loc_row][player.loc_col] = ' ';

            if (choice == 'w') {
                player.direction = 90;
                player.next_col = player.loc_col;
                player.next_row = player.loc_row-1;
                }
            else if (choice == 'a') {
                player.direction = 180;
                player.next_col = player.loc_col-1;
                player.next_row = player.loc_row;
                }
            else if (choice == 's') {
                player.direction = 270;
                player.next_col = player.loc_col;
                player.next_row = player.loc_row+1;
                }
            else if (choice == 'd') {
                player.direction = 0;
                player.next_col = player.loc_col+1;
                player.next_row = player.loc_row;
                }

            status = move(&player, grid);

            if (status == 2) {
                grid[player.next_row][player.next_col] = 1;
                grid[player.loc_row][player.loc_col] = ' ';
                win = 1;
                }
            else if (status == 5) {
                // find all barriers and replace them with blank spots
                for (i = 0; i < 20; i++) {
                    for (j = 0; j < 40; j++) {
                        if (grid[i][j] == 'X')
                            grid[i][j] = ' ';
                        }
                    }
                grid[player.loc_row][player.loc_col] = 'O';
                status = 0;
                }
            else
                grid[player.loc_row][player.loc_col] = 'O';

            }
        if (choice != 'q') {
            // print map after
            for (i = 0; i < 20; i++) {
                for (j = 0; j < 40; j++)
                    superGrid[i+3][j+1] = grid[i][j];
                }

            for (i = 0; i < 24; i++) {
                if (i == 1 && win == 1)
                    printf(" ::TURN %2d::         | PLAYER = {%d,%d}\n", turn+1, player.next_row, player.next_col);
                else if (i == 1 && win != 1)
                    printf(" ::TURN %2d::         | PLAYER = {%d,%d}\n", turn+1, player.loc_row, player.loc_col);
                else
                    printf("%s", superGrid[i]);
                }
            }

        if (status == 0) {
            choice = getChoice();
            }

        else if (status == 1) {
            printf("You have died. Resetting the map.   ");
            system("pause");
            choice = 'e';
            turn = -1;
            status = 0;
            }

        turn++;

        } // won or quit this game

    fclose(map);

    if (win == 1) {
        return turn;
        }

    return 0;

}
