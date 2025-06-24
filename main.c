#include "playmusic.h"
#include <stdio.h>
#include <ncurses.h>

int main() {
    // Init ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // Non-blocking input
    keypad(stdscr, TRUE);
    curs_set(0);

    char* filename = "song.mp3"; // Replace with your music file
    if (playMusic(filename) != 0) {
        fprintf(stderr, "Failed to play music\n");
        return 1;
    }
    return 0;
}
