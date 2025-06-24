#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include "playmusic.h"

char* getMusicDurationFormatted(double seconds, double totalSeconds) {
    char* str = malloc(50);
    if (str) {
        int minutes = (int)(seconds / 60);
        int secs = (int)(seconds) % 60;
        int totalMinutes = (int)(totalSeconds / 60);
        int totalSecs = (int)(totalSeconds) % 60;
        snprintf(str, 50, "%d:%02d/%d:%02d", minutes, secs, totalMinutes, totalSecs);
    }
    return str;
}

char* generateSliderFromPercentage(double percentage, int length) {
    char* slider = malloc(length + 3);
    slider[0] = '[';
    int filledLength = (int)(percentage * length);
    for (int i = 0; i < length; i++) {
        if (i < filledLength) {
            slider[i+1] = '=';
        } else {
            slider[i+1] = '-';
        }
    }
    slider[length+1] = ']';
    return slider;
}


int playMusic(char* filename, char** filesList, int fileCount) {
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music) {
        fprintf(stderr, "Mix_LoadMUS error: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(music, -1);

    bool isPlaying = true;
    while (isPlaying) {
        clear();

        double pos = Mix_GetMusicPosition(music);
        if (pos >= 0) {
            char* duration = getMusicDurationFormatted(pos, Mix_MusicDuration(music));
            mvprintw(1, 1, "%s", duration);
            free(duration);
            char* slider = generateSliderFromPercentage(pos / Mix_MusicDuration(music), 50);
            mvprintw(2, 1, "%s", slider);
            free(slider);
        }
        else
            mvprintw(0, 0, "Playback position: Unknown");

        mvprintw(3, 1, "File name : %s", filename);

        for (int i = 0; fileCount > i; i++) {
            if (filesList[i] != NULL) {
                if (filesList[i] == filename) {
                    attron(COLOR_PAIR(1));
                    mvprintw(5 + i, 1, "-> %s", filesList[i]);
                    attroff(COLOR_PAIR(1));
                } else {;
                    mvprintw(5 + i, 1, "%s", filesList[i]);
                }
            }
        }

        refresh();

        int ch = getch();
        switch (ch) {
            case 'p':
                Mix_PauseMusic();
                break;
            case 'r':
                Mix_ResumeMusic();
                break;
            case 'f':
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) + 10);
                break;
            case 'b':
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) - 10);
                break;
            case 'n':
                isPlaying = false;
                Mix_FreeMusic(music);
                return 1;

            default:
                break;
        }

        usleep(50000); // Sleep for 50ms
    }

    return 0;
}

