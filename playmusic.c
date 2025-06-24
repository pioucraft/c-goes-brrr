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


int playMusic(char* filename) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL init error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio error: %s\n", Mix_GetError());
        return 1;
    }

    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music) {
        fprintf(stderr, "Mix_LoadMUS error: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(music, -1);

    while (1) {
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

        refresh();

        int ch = getch();
        switch (ch) {
            case 'p':
                Mix_PauseMusic();
                break;
            case 'r':
                Mix_ResumeMusic();
                break;
            case 's':
                Mix_HaltMusic();
                Mix_PlayMusic(music, -1);
                break;
            case 'q':
                goto end;
                break;
            case 'f':
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) + 10);
            case 'b':
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) - 10);
            default:
                break;
        }

        usleep(50000); // Sleep for 50ms
    }

end:
    endwin();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}

