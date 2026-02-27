#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include "levels.h"

void printStartPage(){
  printw("=====================================\n");
  printw("|                                   |\n");
  printw("| C. Villani 2026                   |\n");
  printw("|                                   |\n");
  printw("|                                   |\n");
  printw("| Use arrow keys to move the object |\n");
  printw("|                                   |\n");
  printw("| Press 's' to start                |\n");
  printw("|                                   |\n");
  printw("| Press 'q' to leave the game       |\n");
  printw("|                                   |\n");
  printw("=====================================\n");
}

int addResult(double);
void printLevel(int);


int main() {
    initscr();              // Start ncurses mode
    noecho();               // Don't show typed characters
    curs_set(0);            // Hide the cursor
    keypad(stdscr, TRUE);   // Enable arrow keys

    int x0 = 1, y0 = 1;   // Starting position
    int x, y;               // Position
    int ch;
    time_t start_time;
    nPoints = 0;
    nLife = 3;

    printStartPage();

    while ((ch = getch()) != 's'){
      if(ch == 'q'){
        printw("Exiting\n");
        refresh();            // Update the screen
        endwin();             // Restore terminal
        return 0;
      }
    }

    nodelay(stdscr, TRUE);  // Allow non blocking
    start_time =time(NULL);

    for(int level = 0; level < NUM_LEVELS; level++){
      x = x0;
      y = y0;
      clear();
      printLevel(level);
      refresh();            // Update the screen
      mvaddch(y, x, '@');   // Draw the character
      refresh();            // Update the screen
      nColl = 0;
      timeout(100);
      while (nColl < NUM_OBJ) {
          ch = getch();
          if(ch == 'q')
            break;
          // mvaddch(y, x, ' ');  // Erase previous character
          int old_x = x;
          int old_y = y;
          seconds = difftime(time(NULL), start_time);
          // This part is not very good!
          printLevel(level);
          refresh();

          switch(ch) {
              case KEY_UP:    y--; break;
              case KEY_DOWN:  y++; break;
              case KEY_LEFT:  x--; break;
              case KEY_RIGHT: x++; break;
          }

          if(levels[level][y][x] == 'O'){
            nColl++;
            nPoints += 2;
            levels[level][y][x] = ' ';  // remove object from the level
            printLevel(level);
            mvaddch(old_y, old_x, ' '); // Erase previous character
            mvaddch(y, x, '@');         // Draw new character
          }
          else if(levels[level][y][x] == '#'){
            levels[level][y][x] = 'X';
            nLife--;
            printLevel(level);
            refresh();
            // mvaddch(old_y, old_x, '@');
          }
          else{
            // Erase previous character
            mvaddch(old_y, old_x, levels[level][old_y][old_x]);
            mvaddch(y, x, '@');         // Draw new character
          }
          refresh();           // Update the screen
          if(nLife < 1){
            printw("The end\n");
            refresh();            // Update the screen
            sleep(3);
            endwin();             // Restore terminal
            return 0;
          }
      }
      if(ch == 'q'){
        printw("Exiting\n");
        refresh();            // Update the screen
        sleep(3);
        endwin();             // Restore terminal
        return 0;
      }
    }

    clear();
    printw("Congratulations: you won!\n");
    printw("");
    printw("Time: %.2f seconds\n", seconds);
    refresh();
    sleep(1);
    endwin(); // Restore terminal

    int result = addResult(seconds);
    return result;
}

