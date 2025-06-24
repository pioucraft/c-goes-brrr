#include "playmusic.h"
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int main() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // Non-blocking input
    keypad(stdscr, TRUE);
    curs_set(0);

    // get the folder content
    DIR* dir = opendir(".");

    char** fileList = NULL;
    int fileCount = 0;

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL) {
        // if it's not a file with .mp3 extension, skip it
        if (entry->d_type != DT_REG || !strstr(entry->d_name, ".mp3")) {
            continue;
        }
        fileList = realloc(fileList, sizeof(entry->d_name) * (fileCount + 1));
        fileList[fileCount] = strdup(entry->d_name);
        printf("Found file: %s\n", entry->d_name);
        fileCount++;
    }
    closedir(dir);

        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL init error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio error: %s\n", Mix_GetError());
        return 1;
    }


    int filesIndex = 0;
    while(1) {
        char* filename = fileList[filesIndex % fileCount];
        playMusic(filename);
        filesIndex++;
    }

    free(fileList);
    return 0;
}
