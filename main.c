#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <ncurses.h>

int main() {
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Music* music = Mix_LoadMUS("song.mp3");
    if (!music) {
        printf("Error: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(music, -1); // Loop forever
    printf("Playing music. Press p = pause, r = resume, s = stop, q = quit\n");

    char command;
    while (1) {
        command = getchar();

        switch (command) {
            case 'p':
                Mix_PauseMusic();
                printf("Paused\n");
                break;
            case 'r':
                Mix_ResumeMusic();
                printf("Resumed\n");
                break;
            case 's':
                Mix_HaltMusic();
                printf("Stopped\n");
                break;
            case 'f':
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) + 10);
                printf("Skipped 10 seconds\n");
                break;
            case 'b':
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) - 10);
                printf("Rewound 10 seconds\n");
                break;
            case 'q':
                goto end;
        }
    }

end:
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
