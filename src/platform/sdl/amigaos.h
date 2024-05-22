/*
** smarkusg: part of the code comes from mplayer 1.5 AOS
** License: GPL v2 or later
*/

#include <exec/types.h>
#include <SDL2/SDL.h>

#define AMIGA_VERSION "mGBA 0.10.3 (11.05.2024)"

int AmigaOS_Open(int argc, char *argv[]); // returns -1 if a problem
void AmigaOS_Close(void);
void AmigaOS_ParseArg(int argc, char *argv[], int *new_argc, char ***new_argv);
void VARARGS68K EasyRequester(CONST_STRPTR text, CONST_STRPTR button, ...);
void SDLAcaption(SDL_Window *window, char *filename);
