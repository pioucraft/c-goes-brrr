gcc main.c playmusic.c -o a.out -lSDL2 -lSDL2_mixer -lncurses
mv a.out playmusic 
sudo mv playmusic /usr/local/bin
